#pragma once

#include <vector>
#include "PlayerData.h"

struct ConnectionAvailable {
	std::string serverName;
	unsigned int playerCount;
};

struct ConnectionRequest {
	PlayerData player;
};

struct Disconnect {
	uint64_t uuid;
};

// Change that later
struct ConnectionAccept {
	std::vector<PlayerData> players;
};

inline sf::Packet& operator<<(sf::Packet& packet, const ConnectionAccept& conn) {
	return packet << conn.players;
}

inline sf::Packet& operator>>(sf::Packet& packet, ConnectionAccept& conn) {
	return packet >> conn.players;
}

inline sf::Packet& operator<<(sf::Packet& packet, const ConnectionAvailable& conn) {
	return packet << conn.serverName << conn.playerCount;
}

inline sf::Packet& operator>>(sf::Packet& packet, ConnectionAvailable& conn) {
	return packet >> conn.serverName >> conn.playerCount;
}

inline sf::Packet& operator<<(sf::Packet& packet, const Disconnect& conn) {
	return packet << conn.uuid;
}

inline sf::Packet& operator>>(sf::Packet& packet, Disconnect& conn) {
	return packet >> conn.uuid;
}
