#include "Building.h"

Building::Building(float x, float y)
	: m_Rect({100, 100}) {
	m_Rect.setPosition(x, y);
	m_Rect.setOrigin(m_Rect.getSize().x / 2, m_Rect.getSize().y / 2);
}

void Building::OnDraw(sf::RenderWindow &window) {
	window.draw(m_Rect);
}

void Building::SetProduction(bool production) {
	sf::Color color = m_Rect.getFillColor();
	if (production)
		color.a = 128;
	else
		color.a = 255;
	m_Rect.setFillColor(std::move(color));
}
