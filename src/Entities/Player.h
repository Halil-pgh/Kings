#pragma once

#include "Entity.h"
#include "Buildings/Building.h"

class Player : public Entity {
public:
	Player();
	~Player() override = default;

	void OnAttach() override {}
	void OnDetach() override {}
	void OnUpdate(float deltaTime) override {};
	void OnDraw(sf::RenderWindow& window) override;
	bool OnEvent(const sf::Event& event) override;

	void SetName(const std::string& name);
	const std::string& GetName() const { return m_Name; }
	void SetPosition(const sf::Vector2f& position);
	void AddBuilding(const std::shared_ptr<Building>& building) { m_Buildings.push_back(building); }

	// Serialization
	template<class Archive>
	void serialize(Archive& archive) {
		archive(m_Rect, m_Text, m_Name, m_Buildings);
	}

protected:
	sf::RectangleShape m_Rect;
	sf::Text m_Text;
	std::string m_Name;
	sf::Vector2f m_Velocity;

	std::vector<std::shared_ptr<Building>> m_Buildings;

	// Don't know why I need it anyway
	friend class Self;
};