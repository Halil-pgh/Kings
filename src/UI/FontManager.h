#pragma once

#include <SFML/Graphics/Font.hpp>
#include <unordered_map>
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

	static void SetFont(const std::string& name, const std::string& fileName) {
		if (!Get()->m_Fonts[name].loadFromFile(fileName)) {
			std::cout << "Failed to load font from file: " << fileName << "!\n";
			exit(-1);
		}
	}

	inline static const sf::Font& GetFont(const std::string& name) {
		return Get()->m_Fonts[name];
	}

private:
	FontManager() = default;

private:
	static FontManager* s_Instance;
	std::unordered_map<std::string, sf::Font> m_Fonts;
};

inline FontManager* FontManager::s_Instance;