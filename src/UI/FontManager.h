#pragma once

#include <SFML/Graphics/Font.hpp>
#include <iostream>

class FontManager {
public:
	static FontManager* Get() {
		if (!s_Instance)
			s_Instance = new FontManager();
		return s_Instance;
	}

	static void Destroy() {
		delete s_Instance;
	}

	static void SetFont(const std::string& fileName) {
		if (!Get()->m_Font.loadFromFile(fileName)) {
			std::cout << "Failed to load font from file: " << fileName << "!\n";
			exit(-1);
		}
	}

	inline static const sf::Font& GetFont() {
		return Get()->m_Font;
	}

private:
	FontManager() = default;

private:
	static FontManager* s_Instance;
	sf::Font m_Font;
};

inline FontManager* FontManager::s_Instance;