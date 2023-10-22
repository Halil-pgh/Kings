#include "Building.h"

Building::Building(float x, float y, sf::Color color)
	: m_Color(color), m_Rect({100, 100}) {
	m_Rect.setPosition(x, y);
	m_Rect.setFillColor(m_Color);
	m_Rect.setOrigin(m_Rect.getSize().x / 2, m_Rect.getSize().y / 2);
}

void Building::OnDraw(sf::RenderWindow &window) {
	window.draw(m_Rect);
}
