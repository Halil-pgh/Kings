#pragma once

#include <cstdint>

enum class PacketType : uint8_t {
	Join, // For Player
	Leave, // For uint16_t id
	Move, // For sf::Vector2f
	CreateHome, // For Home
	CreateMine, // For Mine
	Chat, // For std::string
};

template <typename T>
struct Packet {
	PacketType type;
	uint16_t id; // Client ID, who's sending the packet, given by the server
	T data;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(type, id, data);
	}
};