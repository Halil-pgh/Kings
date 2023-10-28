#pragma once

#include "Entities/Entity.h"

class PlayerStats : public Entity {
public:
	PlayerStats();

	void SetMoney(uint32_t money);
	void SetMaxSoldierCount(uint32_t count);

	void OnAttach() override {}
	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow &window) override;
	bool OnEvent(const sf::Event &event) override;

private:
	sf::Text m_MaxSoldierText;
	sf::Text m_MoneyText;
};
