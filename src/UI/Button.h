#pragma once

#include "Entities/Entity.h"

class Button : public Entity
{
public:
	Button(float x, float y, float width, float height, const std::string& mess);

	inline void SetNormalColor(const sf::Color& color) { m_NormalColor = color; }
	inline void SetHighlightedColor(const sf::Color& color) { m_HighlightedColor = color; }
	inline void SetPressedColor(const sf::Color& color) { m_PressedColor = color; }
	inline void SetOnClickCallback(const std::function<void()>& func) { m_OnClick = func; }
	inline void SetActive(bool active) { m_Active = active; }

	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow& window) override;
	void OnEvent(const sf::Event& event) override;

private:
	bool isMouseOn();

private:
	sf::RectangleShape m_Rect;
	sf::Text m_Text;

	std::function<void()> m_OnClick;

	bool m_Active = true;
	sf::Color m_NormalColor;
	sf::Color m_HighlightedColor;
	sf::Color m_PressedColor;
};