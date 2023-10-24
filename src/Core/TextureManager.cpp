#include "pch.h"
#include "TextureManager.h"

#include <cassert>

TextureManager* TextureManager::s_Instance = nullptr;

TextureManager* TextureManager::Get() {
	if (s_Instance == nullptr)
		s_Instance = new TextureManager();
	return s_Instance;
}

void TextureManager::AddTexture(const std::string &name, const std::string &texturePath) {
	sf::Texture texture;
	if (!texture.loadFromFile(texturePath)) {
		std::cout << "Failed to load " << texturePath << "!\n";
		assert(false);
	}

	Get()->m_Textures[name] = std::move(texture);
}

sf::Texture* TextureManager::GetTexture(const std::string &name) {
	return &Get()->m_Textures[name];
}
