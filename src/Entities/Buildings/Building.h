#pragma once

#include "Networking/Data/BuildingData.h"
#include "Entities/Entity.h"

class Building : public Entity {
public:
	Building(float x, float y);
	virtual ~Building() = default;

	virtual void OnUpdate(float deltaTime) {}
	void OnDraw(sf::RenderWindow &window) override;
	virtual void OnEvent(const sf::Event &event) {}

	inline const sf::Vector2f& GetPosition() const { return m_Rect.getPosition(); }
	inline void SetPositon(const sf::Vector2f& position) { m_Rect.setPosition(position); }

	void SetProduction(bool production);

protected:
	sf::RectangleShape m_Rect;
	sf::Texture m_Texture;
};
