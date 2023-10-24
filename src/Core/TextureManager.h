#pragma once

class TextureManager {
public:
	static TextureManager* Get();
	static void AddTexture(const std::string& name, const std::string& texturePath);
	static sf::Texture* GetTexture(const std::string& name);

private:
	TextureManager() = default;

private:
	std::unordered_map<std::string, sf::Texture> m_Textures;
	static TextureManager* s_Instance;
};
