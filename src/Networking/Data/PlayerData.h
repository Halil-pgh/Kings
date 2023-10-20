#pragma once

#include <string>
#include "PacketUtils.h"
#include "BuildingData.h"

struct PlayerData {
	uint64_t uuid;
	std::string name;
	sf::Color color;
	sf::Vector2f position;
	float rotation;

    std::vector<BuildingData> buildings;
};

inline sf::Packet& operator<<(sf::Packet& packet, const PlayerData& data) {
	return packet << (sf::Uint64)data.uuid << data.name << data.color << data.position << data.rotation
        << data.buildings;
}

inline sf::Packet& operator>>(sf::Packet& packet, PlayerData& data) {
	sf::Uint64 uuid;
	packet >> uuid >> data.name >> data.color >> data.position >> data.rotation >> data.buildings;
	data.uuid = uuid;
	return packet;
}