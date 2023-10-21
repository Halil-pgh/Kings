#include "Building.h"

Building::Building(float x, float y, sf::Color color)
	: m_Color(color), m_Rect({200, 200}) {
	m_Rect.setPosition(x, y);
	m_Rect.setFillColor(m_Color);
}

void Building::OnDraw(sf::RenderWindow &window) {
	window.draw(m_Rect);
}
