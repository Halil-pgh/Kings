#include "pch.h"
#include "Home.h"

#include "Core/TextureManager.h"

Home::Home(float x, float y)
	:Building(x, y, BuildingType::Home) {
	m_Rect.setTexture(TextureManager::GetTexture(TextureName()));
}

Home::Home() {
	m_Rect.setTexture(TextureManager::GetTexture(TextureName()));
}

void Home::OnUpdate(float deltaTime) {

}

bool Home::OnEvent(const sf::Event &event) {
	return false;
}

std::string Home::TextureName() {
	return "home";
}

unsigned int Home::GetMaxCount() {
	return 10;
}
