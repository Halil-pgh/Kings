#pragma once

#include <vector>
#include "PlayerData.h"

struct ConnectionAvailable
{
	std::string serverName;
	unsigned int playerCount;
};

struct ConnectionRequest
{
	PlayerData player;
};

// Change that later
struct ConnectionAccept
{
	std::string serverName;
	unsigned int playerCount;
};

inline sf::Packet& operator<<(sf::Packet& packet, const ConnectionAccept& conn)
{
	return packet << conn.serverName << conn.playerCount;
}

inline sf::Packet& operator>>(sf::Packet& packet, ConnectionAccept& conn)
{
	return packet >> conn.serverName >> conn.playerCount;
}

inline sf::Packet& operator<<(sf::Packet& packet, const ConnectionAvailable& conn)
{
	return packet << conn.serverName << conn.playerCount;
}

inline sf::Packet& operator>>(sf::Packet& packet, ConnectionAvailable& conn)
{
	return packet >> conn.serverName >> conn.playerCount;
}