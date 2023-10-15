#pragma once

#include <SFML/Graphics/Font.hpp>
#include <iostream>

class FontManager
{
public:
	static void SetFont(const std::string& fileName)
	{
		if (!s_Font.loadFromFile(fileName))
		{
			std::cout << "Failed to load font from file: " << fileName << "!\n";
			exit(-1);
		}
	}

	inline static const sf::Font& GetFont()
	{
		return s_Font;
	}

private:
	static sf::Font s_Font;
};

inline sf::Font FontManager::s_Font;