#include "pch.h"
#include "Home.h"

#include "Core/TextureManager.h"

Home::Home(float x, float y)
	:Building(x, y) {
	m_Rect.setTexture(TextureManager::GetTexture("home"));
}

void Home::OnUpdate(float deltaTime) {

}

void Home::OnEvent(const sf::Event &event) {

}

