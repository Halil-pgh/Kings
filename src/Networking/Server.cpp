#include "pch.h"
#include "Server.h"

#include <cassert>
#include <utility>
#include "Data/Types.h"
#include "Data/Connections.h"
#include "Data/ServerData.h"

Server::Server(std::string name)
	: m_Name(std::move(name)) {
	m_Data.uuid = GenerateUUID();

	if (m_Socket.bind(PORT) != sf::Socket::Done) {
		std::cout << "Failed to bind the port " << PORT << "!\n";
		assert(false);
	}
	m_Socket.setBlocking(false);

	// And it is not...
	m_Players.push_back(m_Data);
}

Server::~Server() {
	ShoutDown();
}

void Server::Run() {
	m_Running = true;
	m_Thread = std::thread([&]() {

		// Warning: m_Clients and m_Players are needs to sync!
		while (m_Running) {
			sf::IpAddress clientIp;
			unsigned short clientPort;
			sf::Packet packet;
			if ((m_Socket.receive(packet, clientIp, clientPort) != sf::Socket::NotReady) && packet) {
				unsigned int typeInt;
				packet >> typeInt;
				auto type = (DataTypes)typeInt;
				if (type == DataTypes::ConnectionRequest) {
					PlayerData data;
					packet >> data;

					// send connection request for other m_Clients
					packet.clear();
					packet << (unsigned int)DataTypes::ConnectionRequest;
					packet << data;
					for (const auto& client : m_Clients) {
						if (m_Socket.send(packet, client.ip, client.port) != sf::Socket::Done) {
							std::cout << "Failed to send client " << client.ip << ":" << client.port << " request!\n";
							assert(false);
						}
					}

					// Connection accept filled before this client added!
					ConnectionAccept accept;
					{
						std::lock_guard<std::mutex> lock(m_PlayersMutex);
						accept.players = m_Players;
					}

					m_Clients.push_back({
						clientIp,
						clientPort
					});
					{
						std::lock_guard<std::mutex> lock(m_PlayersMutex);
						m_Players.push_back(data);
					}
					m_OnConnect(data);

					// send connection accept to the client
					packet.clear();
					packet << (unsigned int)DataTypes::ConnectionAccept;
					packet << accept;
					if (m_Socket.send(packet, clientIp, clientPort) != sf::Socket::Done) {
						std::cout << "Failed to send client " << clientIp << ":" << clientPort << " accepted!\n";
						assert(false);
					}
				}
				else if (type == DataTypes::PlayerData) {
					if (!packet)
						break;

					PlayerData data;
					packet >> data;

					{
						std::lock_guard<std::mutex> lock(m_PlayersMutex);
						for (PlayerData& player : m_Players)
							if (player.uuid == data.uuid)
								player = data;
					}
				}
				else if (type == DataTypes::ConnectionAvailableRequest) {
					ConnectionAvailable conn = {
						m_Name,
						(unsigned int)m_Players.size()
					};

					std::cout << "Connection available...\n";

					packet.clear();
					packet << (unsigned int)DataTypes::ConnectionAvailable;
					packet << conn;

					if (m_Socket.send(packet, clientIp, clientPort) != sf::Socket::Done) {
						std::cout << "Failed to send client " << clientIp << ":" << clientPort << " available!\n";
						assert(false);
					}
				}
				else if (type == DataTypes::Disconnect) {
					Disconnect data{};
					packet >> data;
					m_OnDisconnect(data.uuid);

					{
						std::lock_guard<std::mutex> lock(m_PlayersMutex);
						for (uint32_t i = 0; i < m_Players.size(); i++) {
							if (m_Players[i].uuid == data.uuid) {
								m_Clients.erase(m_Clients.begin() + i - 1);
								m_Players.erase(m_Players.begin() + i);
								break;
							}
						}
					}

					packet.clear();
					packet << (unsigned int)DataTypes::Disconnect;
					packet << data;
					for (ClientData& client : m_Clients) {
						if (m_Socket.send(packet, client.ip, client.port) != sf::Socket::Done) {
							std::cout << "Failed to send disconnect data to " << client.ip << ":" << client.port << "\n";
							assert(false);
						}
					}
				}
			}

			packet.clear();
			packet << (unsigned int)DataTypes::ServerData;

			ServerData serverData;
			{
				std::lock_guard<std::mutex> lock(m_PlayersMutex);
				serverData.players = m_Players;
			}

			packet << serverData;

			for (ClientData& client : m_Clients) {
				if (m_Socket.send(packet, client.ip, client.port) != sf::Socket::Done) {
					std::cout << "Failed to send players data to " << client.ip << ":" << client.port << "\n";
					assert(false);
				}
			}
		}
	});
}

void Server::SetPlayerData(const PlayerData &data) {
	{
		std::lock_guard<std::mutex> lock(m_PlayersMutex);
		m_Players[0] = data;
	}
	m_Data = data;
}

void Server::ShoutDown() {
	sf::Packet packet;
	Disconnect data { GetUUID() };

	// Server disconnect has to be something else
	packet << (unsigned int)DataTypes::DisconnectServer;
	packet << data;
	for (const auto& client : m_Clients) {
		if (m_Socket.send(packet, client.ip, client.port) != sf::Socket::Done) {
			std::cout << "Failed to send disconnect data to client " << client.ip << ":" << client.port << "\n";
			assert(false);
		}
	}

	m_Running = false;
	if (m_Thread.joinable())
		m_Thread.join();
	m_Socket.unbind();
}
