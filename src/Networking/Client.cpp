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
	m_Running = false;
	if (m_Thread.joinable()) {
		m_Thread.join();
	}
}

void Client::Run() {
	m_Running = true;
	m_Thread = std::thread([&]() {
		sf::IpAddress serverIp;
		unsigned short serverPort;

		sf::IpAddress receivedIP;
		unsigned short receivedPort;

		while (m_Running) {
			sf::Packet packet;
			if (!m_Connected) {
				if (m_Socket.receive(packet, serverIp, serverPort) != sf::Socket::NotReady) {
					unsigned int typeInt;
					packet >> typeInt;
					auto type = (DataTypes)typeInt;
					if (type == DataTypes::ConnectionAccept) {
						std::cout << "Connected to server " << serverIp << ":" << serverPort << "\n";

						m_Connected = true;
						ConnectionAccept accept;
						packet >> accept;
					}
					else if (type == DataTypes::ConnectionAvailable) {
						std::cout << "Connection is available with " << serverIp << ":" << serverPort << "\n";

						ConnectionAvailable available;
						packet >> available;
						m_ServerInfos.push_back({
							serverIp, serverPort, available.serverName, available.playerCount
						});
					}
				}

				continue;
			}

			packet << (unsigned int)DataTypes::PlayerData;
			packet << m_Data;
			if (m_Socket.send(packet, serverIp, Server::PORT) != sf::Socket::Done) {
				std::cout << "Failed to send player data to server " << serverIp << ":" << Server::PORT << "\n";
				assert(false);
			}

			packet.clear();

			if (m_Socket.receive(packet, receivedIP, receivedPort) != sf::Socket::NotReady) {
				unsigned int typeInt;
				packet >> typeInt;
				auto type = (DataTypes)typeInt;
				if (type == DataTypes::ServerData) {
					ServerData serverData;
					packet >> serverData;
					m_Players = serverData.players;
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
}

void Client::ShoutDown() {
	m_Running = false;
	if (m_Thread.joinable()) {
		m_Thread.join();
	}
}
