#pragma once

#include "Networking/Data/BuildingData.h"
#include "Entities/Entity.h"

class Building : public Entity {
public:
	Building(float x, float y, sf::Color color);
	virtual ~Building() = default;

	virtual void OnUpdate(float deltaTime) = 0;
	void OnDraw(sf::RenderWindow &window) override;
	virtual void OnEvent(const sf::Event &event) = 0;

	const sf::Vector2f& GetPosition() const { return m_Rect.getPosition(); }

protected:
	sf::RectangleShape m_Rect;

	// TODO: make this using sf::Image
	sf::Color m_Color;
};
