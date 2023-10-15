#include "pch.h"
#include "Server.h"

#include <cassert>
#include "Data/Types.h"
#include "Data/Connections.h"
#include "Data/ServerData.h"

struct ClientData
{
	sf::IpAddress ip;
	unsigned short port;
};

Server::Server(const std::string& name)
	: m_Name(name)
{
	m_Data.uuid = GenerateUUID();

	if (m_Socket.bind(PORT) != sf::Socket::Done)
	{
		std::cout << "Failed to bind the port " << PORT << "!\n";
		assert(false);
	}
	m_Socket.setBlocking(false);

	// And it is not...
	m_Players.push_back(m_Data);
}

Server::~Server()
{
	if (m_Thread.joinable())
		m_Thread.join();
}

void Server::Run()
{
	m_Thread = std::thread([&]() {

		// Warning: clients and m_Players are needs to synced!
		std::vector<ClientData> clients;
		sf::IpAddress clientIp;
		unsigned short clientPort;

		while (true)
		{
			sf::Packet packet;
			if (m_Socket.receive(packet, clientIp, clientPort) != sf::Socket::NotReady)
			{
				unsigned int typeInt;
				packet >> typeInt;
				DataTypes type = (DataTypes)typeInt;

				if (type == DataTypes::ConnectionRequest)
				{
					PlayerData data;
					packet >> data;

					clients.push_back({
						clientIp,
						clientPort
					});
					m_Players.push_back(data);

					packet.clear();

					packet << (unsigned int)DataTypes::ConnectionAccept;
					ConnectionAccept accept = {
						m_Name,
						m_Players.size()
					};
					packet << accept;

					if (m_Socket.send(packet, clientIp, clientPort) != sf::Socket::Done)
					{
						std::cout << "Failed to send client " << clientIp << ":" << clientPort << " accepted!\n";
						assert(false);
					}
				}
				else if (type == DataTypes::PlayerData)
				{
					PlayerData data;
					packet >> data;

					for (PlayerData& player : m_Players)
						if (player.uuid == data.uuid)
							player = data;
				}
				else if (type == DataTypes::ConnectionAvailableRequest)
				{
					ConnectionAvailable conn = {
						m_Name,
						m_Players.size()
					};

					packet.clear();

					packet << (unsigned int)DataTypes::ConnectionAvailable;
					packet << conn;

					if (m_Socket.send(packet, clientIp, clientPort) != sf::Socket::Done)
					{
						std::cout << "Failed to send client " << clientIp << ":" << clientPort << " available!\n";
						assert(false);
					}
				}
			}

			packet.clear();
			packet << (unsigned int)DataTypes::ServerData;

			ServerData serverData;
			serverData.players = m_Players;

			packet << serverData;

			for (ClientData& client : clients)
			{
				if (m_Socket.send(packet, client.ip, client.port) != sf::Socket::Done)
				{
					std::cout << "Failed to send players data to " << client.ip << ":" << client.port << "\n";
					assert(false);
				}
			}
		}
	});
}
