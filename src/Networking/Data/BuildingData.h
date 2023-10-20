#pragma once

#include "PacketUtils.h"

enum class BuildingType {
    Home,
    Mine
};

struct BuildingData {
    BuildingType type;
    sf::Vector2f position;
};

inline sf::Packet& operator<<(sf::Packet& packet, const BuildingData& data) {
    return packet << static_cast<unsigned int>(data.type) << data.position;
}

inline sf::Packet& operator>>(sf::Packet& packet, BuildingData& data) {
    unsigned int type;
    packet >> type >> data.position;
    data.type = static_cast<BuildingType>(type);
    return packet;
}
