#pragma once

#include <vector>
#include <SFML/Network/Packet.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

template<typename T>
inline sf::Packet& operator<<(sf::Packet& packet, const std::vector<T>& vector) {
	packet << static_cast<int>(vector.size());
	for (const T& t : vector)
		packet << t;
	return packet;
}

template<typename T>
inline sf::Packet& operator>>(sf::Packet& packet, std::vector<T>& vector) {
	int size;
	packet >> size;
	vector.clear();
	vector.reserve(size);
	for (uint32_t i = 0; i < size; i++) {
		T t;
		packet >> t;
		vector.push_back(t);
	}
	return packet;
}

inline sf::Packet& operator<<(sf::Packet& packet, const sf::Color& color) {
	return packet << color.r << color.g << color.b << color.a;
}

inline sf::Packet& operator>>(sf::Packet& packet, sf::Color& color) {
	return packet >> color.r >> color.g >> color.b >> color.a;
}

inline sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& vec) {
	return packet << vec.x << vec.y;
}

inline sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& vec) {
	return packet >> vec.x >> vec.y;
}