#include "pch.h"
#include "Client.h"

#include <cassert>
#include "Data/Types.h"
#include "Data/Connections.h"
#include "Data/ServerData.h"
#include "Server.h"

Client::Client() {
	m_Data.uuid = GenerateUUID();

	if (m_Socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
		std::cout << "Failed to bind port " << m_Socket.getLocalPort() << "!\n";
		assert(false);
	}
	m_Socket.setBlocking(false);
}

Client::~Client() {
	ShoutDown();
}

void Client::Run() {
	m_Running = true;
	m_Thread = std::thread([&]() {
		sf::IpAddress receivedIP;
		unsigned short receivedPort;

		while (m_Running) {
			sf::Packet packet;
			if (!m_Connected) {
				if (m_Socket.receive(packet, m_ServerIp, m_ServerPort) != sf::Socket::NotReady) {
					unsigned int typeInt;
					packet >> typeInt;
					auto type = (DataTypes)typeInt;
					if (type == DataTypes::ConnectionAccept) {
						std::cout << "Connected to server " << m_ServerIp << ":" << m_ServerPort << "\n";

						m_Connected = true;
						ConnectionAccept accept;
						packet >> accept;
						m_Players = accept.players;
						for (const auto& player : m_Players) {
							m_OnConnect(player);
						}
					}
					else if (type == DataTypes::ConnectionAvailable) {
						std::cout << "Connection is available with " << m_ServerIp << ":" << m_ServerPort << "\n";

						ConnectionAvailable available;
						packet >> available;
						m_ServerInfos.push_back({
							m_ServerIp, m_ServerPort, available.serverName, available.playerCount
						});
					}
				}

				continue;
			}

			packet << (unsigned int)DataTypes::PlayerData;
			packet << m_Data;
			if (m_Socket.send(packet, m_ServerIp, Server::PORT) != sf::Socket::Done) {
				std::cout << "Failed to send player data to server " << m_ServerIp << ":" << Server::PORT << "\n";
				assert(false);
			}

			packet.clear();
			if (m_Socket.receive(packet, receivedIP, receivedPort) != sf::Socket::NotReady) {
				if (packet) {
					unsigned int typeInt;
					packet >> typeInt;
					auto type = (DataTypes)typeInt;
					if (type == DataTypes::ServerData) {
						if (packet) {
							ServerData serverData;
							packet >> serverData;
							m_Players = serverData.players;
						}
					}
					else if (type == DataTypes::Disconnect) {
						Disconnect data;
						packet >> data;
						m_OnDisconnect(data.uuid);
					}
					else if (type == DataTypes::ConnectionRequest) {
						PlayerData data;
						packet >> data;
						m_OnConnect(data);
					}
					else if (type == DataTypes::DisconnectServer) {
						m_IsInGame = false;
					}
				}
			}
		}
	});
}

void Client::RefreshServers() {
	if (m_Connected) {
		std::cout << "Failed to refresh servers because client is already connected!\n";
		return;
	}

	sf::Packet packet;
	packet << (unsigned int)DataTypes::ConnectionAvailableRequest;
	packet << m_Data;
	if (m_Socket.send(packet, sf::IpAddress::Broadcast, Server::PORT) != sf::Socket::Done) {
		std::cout << "Failed to broadcast connection available request!\n";
		assert(false);
	}
}

void Client::JoinServer(const ServerInfo& server) {
	sf::Packet packet;
	packet << (unsigned int)DataTypes::ConnectionRequest;
	packet << m_Data;
	if (m_Socket.send(packet, server.ip, server.port) != sf::Socket::Done) {
		std::cout << "Failed to send connection request to " << server.ip << ":" << server.port << "!\n";
		assert(false);
	}
	m_IsInGame = true;
}

void Client::ShoutDown() {
	if (m_Connected) {
		sf::Packet packet;
		Disconnect data { GetUUID() };
		packet << (unsigned int)DataTypes::Disconnect;
		packet << data;
		for (int i = 0; i < 100; i++) {
			if (m_Socket.send(packet, m_ServerIp, Server::PORT) != sf::Socket::Done) {
				std::cout << "Failed to send disconnect data to server " << m_ServerIp << ":" << Server::PORT << "\n";
				assert(false);
			}
		}
		std::cout << "Disconnect message send!\n";
	}

	m_Running = false;
	if (m_Thread.joinable()) {
		m_Thread.join();
	}
	m_Socket.unbind();
}
