#include "Mine.h"

#include "Core/TextureManager.h"

Mine::Mine(float x, float y)
	: Building(x, y, BuildingType::Mine) {
	m_Rect.setTexture(TextureManager::GetTexture(TextureName()));
}

void Mine::OnUpdate(float deltaTime) {

}

bool Mine::OnEvent(const sf::Event &event) {
	return false;
}

std::string Mine::TextureName() {
	return "mine";
}

unsigned int Mine::MoneyPerSecond() {
	return 1;
}
