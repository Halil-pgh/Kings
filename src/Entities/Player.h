#pragma once

#include "Entity.h"
#include "Networking/Data/PlayerData.h"
#include "Buildings/Building.h"

class Player : public Entity {
public:
	Player();
	~Player() override = default;

	void OnAttach() override {}
	void OnUpdate(float deltaTime) override {};
	void OnDraw(sf::RenderWindow& window) override;
	bool OnEvent(const sf::Event& event) override;

	void SetName(const std::string& name);
	void Reload(const PlayerData& data);

protected:
	sf::RectangleShape m_Rect;
	sf::Text m_Text;
	std::string m_Name;
	sf::Vector2f m_Velocity;

	std::vector<Building*> m_Buildings;

	// Don't know why I need it anyway
	friend class Self;
};