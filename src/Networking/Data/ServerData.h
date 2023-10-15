#pragma once

#include <vector>
#include "PlayerData.h"

struct ServerData
{
	std::vector<PlayerData> players;
};

inline sf::Packet& operator<<(sf::Packet& packet, const ServerData& data)
{
	return packet << data.players;
}

inline sf::Packet& operator>>(sf::Packet& packet, ServerData& data)
{
	return packet >> data.players;
}
