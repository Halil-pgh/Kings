#include "pch.h"
#include "Home.h"

#include "Core/TextureManager.h"

Home::Home(float x, float y)
	:Building(x, y) {
	m_Rect.setTexture(TextureManager::GetTexture(TextureName()));
}

void Home::OnUpdate(float deltaTime) {

}

void Home::OnEvent(const sf::Event &event) {

}

std::string Home::TextureName() {
	return "home";
}

unsigned int Home::GetMaxCount() {
	return 10;
}
