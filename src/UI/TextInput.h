#pragma once

#include "Entities/Entity.h"

class TextInput : public Entity {
public:
	TextInput(float x, float y, float width, float height, const std::string& defaultText = "");

	void OnAttach() override {}
	void OnDetach() override {}
	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow& window) override;
	bool OnEvent(const sf::Event& event) override;

	std::string Get();

private:
	bool isMouseOn();

private:
	sf::RectangleShape m_Rect;
	sf::RectangleShape m_Cursor;
	sf::Text m_Text;
	sf::Text m_DefaultText;

	std::string m_Input;
	bool m_TakeInput = false;

	float m_BlinkingSeconds = 0.5f;
	bool m_IsBlinking = false;
	sf::Clock m_Clock;
};