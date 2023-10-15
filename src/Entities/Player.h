#pragma once

#include "Entity.h"
#include "Networking/Data/PlayerData.h"

class Player : public Entity
{
public:
	Player();
	virtual ~Player() = default;

	virtual void OnUpdate(float deltaTime) {};
	virtual void OnDraw(sf::RenderWindow& window) override;
	virtual void OnEvent(const sf::Event& event) {};

	void SetName(const std::string& name);
	void Reload(const PlayerData& data);

protected:
	sf::RectangleShape m_Rect;
	sf::Text m_Text;
	std::string m_Name;
	sf::Vector2f m_Velocity;
};