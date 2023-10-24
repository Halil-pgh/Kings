#include "Mine.h"

#include "Core/TextureManager.h"

Mine::Mine(float x, float y)
	: Building(x, y) {
	m_Rect.setTexture(TextureManager::GetTexture("mine"));
}

void Mine::OnUpdate(float deltaTime) {

}

void Mine::OnEvent(const sf::Event &event) {

}
