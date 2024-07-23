#include "pch.h"
#include "Server.h"

#include "Networker.h"
#include "Packet.h"
#include "Core/Serializator.h"
#include "Entities/Buildings/Home.h"
#include "Entities/Buildings/Mine.h"

Server::Server(uint16_t port) {
	address.host = ENET_HOST_ANY;
	address.port = port;
	host = enet_host_create(&address, 32, 2, 0, 0);
	if (host == nullptr) {
		std::cout << "An error occurred while creating the server.\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "Server started on port " << port << ".\n";
}

Server::~Server() {
	enet_host_destroy(host);
}

void Server::update() {
	ENetEvent event;
	while (enet_host_service(host, &event, 1000) > 0) {
		switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT: {
				std::cout << "[SERVER]: " + Networker::hostToIp(event.peer->address.host) + ":" + std::to_string(event.peer->address.port) + " connected." << "\n";
				std::cout << "[SERVER]: Connected peer ID: " << event.peer->incomingPeerID << "\n";
				break;
			}

			case ENET_EVENT_TYPE_RECEIVE: {
				PacketType type = Serializator::desiriealizePacketType(event.packet->data, event.packet->dataLength);
				if (type == PacketType::Join) {
					for (const auto& [id, entity] : players) {
						Packet<Player> packet = {PacketType::Join, id, *(entity)};
						size_t _size;
						void* buffer = Serializator::serialize(packet, _size);
						send(event.peer, buffer, _size);
						free(buffer);
					}

					Packet<Player> player = Serializator::deserialize<Player>(event.packet->data, event.packet->dataLength);
					players[player.id] = std::make_shared<Player>(player.data);

					broadcastExcept(event.peer, event.packet->data, event.packet->dataLength);
					std::cout << "[SERVER]: " + player.data.GetName() + " joined the server." << "\n";
				} else if (type == PacketType::Leave) {
					Packet<uint16_t> id = Serializator::deserialize<uint16_t>(event.packet->data, event.packet->dataLength);
					std::cout << "[SERVER]: " + players[id.data]->GetName() + " left the server." << "\n";
					broadcastExcept(event.peer, event.packet->data, event.packet->dataLength);
					players.erase(id.data);
				} else if (type == PacketType::Chat) {
					Packet<std::string> packet = Serializator::deserialize<std::string>(event.packet->data, event.packet->dataLength);
					const std::string& message = packet.data;
					// Check if the message is a command.
					if (message[0] == '/') {
						if (message == "/list") {
							std::string list = "[SERVER]: ";
							for (const auto& [_, entity] : players) {
								list += entity->GetName() + ", ";
							}
							list.pop_back();
							list.pop_back();
							Packet<std::string> listPacket = {PacketType::Chat, SERVER_ID, list};
							size_t _size;
							void* buffer = Serializator::serialize(listPacket, _size);
							send(event.peer, buffer, _size);
							free(buffer);
						} else if (message == "/disconnect") {
							std::string disconnectMessage = "[SERVER]: " + players[event.peer->incomingPeerID]->GetName() + " disconnected.";
							std::cout << disconnectMessage << "\n";
							Packet<uint16_t> leavePacket = {PacketType::Leave, event.peer->incomingPeerID, event.peer->incomingPeerID};
							size_t _size;
							void* buffer = Serializator::serialize(leavePacket, _size);
							broadcast(buffer, _size);
							players.erase(event.peer->incomingPeerID);
							disconnect(event.peer);
							free(buffer);
						} else {
							std::string errorMessage = "[SERVER]: Invalid command.";
							Packet<std::string> errorPacket = {PacketType::Chat, SERVER_ID, errorMessage};
							size_t _size;
							void* buffer = Serializator::serialize(errorPacket, _size);
							send(event.peer, buffer, _size);
							free(buffer);
						}
					}
					else {
						std::cout << players[event.peer->incomingPeerID]->GetName() + ": " + message << "\n";
						broadcastExcept(event.peer, event.packet->data, event.packet->dataLength);
					}
				} else if (type == PacketType::Move) {
					Packet<sf::Vector2f> packet = Serializator::deserialize<sf::Vector2f>(event.packet->data, event.packet->dataLength);
					players[event.peer->incomingPeerID]->SetPosition(packet.data);
					broadcastExcept(event.peer, event.packet->data, event.packet->dataLength);
				} else if (type == PacketType::CreateHome) {
					Packet<Home> packet = Serializator::deserialize<Home>(event.packet->data, event.packet->dataLength);
					players[event.peer->incomingPeerID]->AddBuilding(std::make_shared<Home>(packet.data));
					broadcastExcept(event.peer, event.packet->data, event.packet->dataLength);
				} else if (type == PacketType::CreateMine) {
					Packet<Mine> packet = Serializator::deserialize<Mine>(event.packet->data, event.packet->dataLength);
					players[event.peer->incomingPeerID]->AddBuilding(std::make_shared<Mine>(packet.data));
					broadcastExcept(event.peer, event.packet->data, event.packet->dataLength);
				}
				enet_packet_destroy(event.packet);
				break;
			}

			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
			case ENET_EVENT_TYPE_DISCONNECT: {
				std::cout << "[SERVER]: " + Networker::hostToIp(event.peer->address.host) + ":" + std::to_string(event.peer->address.port) + " disconnected." << "\n";
				std::cout << "[SERVER]: Disconnected peer ID: " << event.peer->incomingPeerID << "\n";
				players.erase(event.peer->incomingPeerID);
				Packet<uint16_t> packet = {PacketType::Leave, event.peer->incomingPeerID, event.peer->incomingPeerID};
				size_t size;
				void* buffer = Serializator::serialize(packet, size);
				broadcast(buffer, size);
				free(buffer);
				break;
			}

			case ENET_EVENT_TYPE_NONE:
				break;
		}
	}
}

void Server::send(ENetPeer *peer, const void *data, size_t size) {
	ENetPacket *packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void Server::broadcast(const void *data, size_t size) {
	ENetPacket *packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(host, 0, packet);
}

void Server::broadcastExcept(ENetPeer *peer, const void *data, size_t size) {
	ENetPacket *packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
	for (size_t i = 0; i < host->peerCount; i++) {
		ENetPeer *currentPeer = &host->peers[i];
		if (currentPeer != peer) {
			enet_peer_send(currentPeer, 0, packet);
		}
	}
}

void Server::disconnect(ENetPeer *peer) {
	enet_peer_disconnect(peer, 0);
	ENetEvent event;
	bool disconnected = false;
	if (enet_host_service(host, &event, 3000) > 0 && event.type == ENET_EVENT_TYPE_DISCONNECT) {
		disconnected = true;
	}
	if (!disconnected) {
		enet_peer_reset(peer);
	}
}

bool Server::someClientConnected() {
	return host->connectedPeers > 0;
}
