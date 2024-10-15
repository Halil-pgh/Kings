#pragma once

#include "Entities/Entity.h"

class ShopBar : public Entity {
public:
	ShopBar();

	void OnAttach() override {}
	void OnDetach() override {}
	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow &window) override;
	bool OnEvent(const sf::Event &event) override;

private:
	std::vector<sf::RectangleShape> m_Rects;
	std::vector<char> m_Keys;
	std::vector<sf::Text> m_KeysText;
	std::vector<sf::Text> m_MoneyTexts;
	std::vector<uint32_t> m_Moneys;
};
