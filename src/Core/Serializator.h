#pragma once

#include <sstream>
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/types/memory.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "UI/FontManager.h"

#include "Network/Packet.h"

// Example of usage:
// #include <vector>
// #include <cereal/types/vector.hpp>
// struct Test {
// 	int a;
// 	float b;
// 	std::vector<int> c;
// 	void print() {
// 		std::cout << a << ", " << b << "\n";
// 		for (int i : c) {
// 			std::cout << i << "-";
// 		}
// 		std::cout << "\n";
// 	}
// 	template<class Archive>
// 	void serialize(Archive& archive) {
// 		archive(a, b, c);
// 	}
// };
// Test original = {1, 2.0f, {3, 4, 5}};
// size_t size;
// void* buffer = Serializator::serialize(original, size);
// Test deserialized = Serializator::deserialize<Test>(buffer, size);
// original.print();
// deserialized.print();
// free(buffer);

// Utility functions for serializing SFML classes.
namespace cereal {
	template<class Archive>
	void serialize(Archive& archive, sf::Vector2f& vector) {
		archive(vector.x, vector.y);
	}

	template<class Archive>
	void serialize(Archive& archive, sf::String& string) {
		std::string str = string.toAnsiString();
		archive(str);
		string = str;
	}

	template<class Archive, typename T>
	void serialize(Archive& archive, sf::Vector2<T>& vector) {
		archive(vector.x, vector.y);
	}

	template<class Archive>
	void serialize(Archive& archive, sf::Color& color) {
		archive(color.r, color.g, color.b, color.a);
	}

	template<class Archive>
	void save(Archive& archive, const sf::RectangleShape& rectangleShape) {
		archive(rectangleShape.getPosition(), rectangleShape.getSize(), rectangleShape.getRotation(), rectangleShape.getScale(), rectangleShape.getFillColor(), rectangleShape.getOrigin());
	}

	template<class Archive>
	void load(Archive& archive, sf::RectangleShape& rectangleShape) {
		sf::Vector2f position, size, scale, origin;
		sf::Color fillColor;
		float rotation;
		archive(position, size, rotation, scale, fillColor, origin);
		rectangleShape.setPosition(position);
		rectangleShape.setSize(size);
		rectangleShape.setRotation(rotation);
		rectangleShape.setScale(scale);
		rectangleShape.setFillColor(fillColor);
		rectangleShape.setOrigin(origin);
	}

	template<class Archive>
	void save(Archive& archive, const sf::Text& text) {
		archive(text.getString(), text.getPosition(), text.getStyle(), text.getCharacterSize(), text.getRotation(), text.getScale(), text.getFillColor(), text.getOrigin());
	}

	template<class Archive>
	void load(Archive& archive, sf::Text& text) {
		std::string string;
		sf::Vector2f position, scale, origin;
		unsigned int characterSize;
		float rotation;
		sf::Uint32 style;
		sf::Color fillColor;
		archive(string, position, style, characterSize, rotation, scale, fillColor, origin);
		text.setString(string);
		text.setPosition(position);
		text.setStyle(style);
		text.setFont(FontManager::GetFont("normal"));
		text.setCharacterSize(characterSize);
		text.setRotation(rotation);
		text.setScale(scale);
		text.setFillColor(fillColor);
		text.setOrigin(origin);
	}
}

class Serializator {
public:
	template<typename T>
	static void* serialize(const Packet<T>& obj, size_t& size) {
		std::ostringstream oss;
		{
			cereal::BinaryOutputArchive oarchive(oss);
			oarchive(obj);
		}

		std::string serializedData = oss.str();
		size = serializedData.size();
		void* buffer = malloc(size);
		memcpy(buffer, serializedData.data(), size);

		return buffer;
	}

	template<typename T>
	static Packet<T> deserialize(void* buffer, size_t size) {
		std::string serializedData(static_cast<char*>(buffer), size);
		std::istringstream iss(serializedData);
		Packet<T> obj;
		{
			cereal::BinaryInputArchive iarchive(iss);
			iarchive(obj);
		}
		return obj;
	}

	static PacketType desiriealizePacketType(void* buffer, size_t size) {
		std::string serializedData(static_cast<char*>(buffer), size);
		std::istringstream iss(serializedData);
		PacketType packetType;
		{
			cereal::BinaryInputArchive iarchive(iss);
			iarchive(packetType);
		}
		return packetType;
	}
};
