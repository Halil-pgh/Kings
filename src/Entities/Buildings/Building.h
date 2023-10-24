#pragma once

#include "Networking/Data/BuildingData.h"
#include "Entities/Entity.h"

class Building : public Entity {
public:
	Building(float x, float y, sf::Color color);
	virtual ~Building() = default;

	virtual void OnUpdate(float deltaTime) {}
	void OnDraw(sf::RenderWindow &window) override;
	virtual void OnEvent(const sf::Event &event) {}

	inline const sf::Vector2f& GetPosition() const { return m_Rect.getPosition(); }
	inline void SetPositon(const sf::Vector2f& position) { m_Rect.setPosition(position); }

	void SetProduction(bool production) {
		sf::Color color;
		if (production)
			color = sf::Color(m_Rect.getFillColor().r, m_Rect.getFillColor().g, m_Rect.getFillColor().b, 128);
		else
			color = sf::Color(m_Rect.getFillColor().r, m_Rect.getFillColor().g, m_Rect.getFillColor().b, 255);
		m_Rect.setFillColor(std::move(color));
	}

protected:
	sf::RectangleShape m_Rect;

	// TODO: make this using sf::Image
	sf::Color m_Color;
};
