#include "pch.h"
#include "TextInput.h"

#include "Core/Application.h"
#include "FontManager.h"

#define MID_POS_X (m_Rect.getPosition().x + m_Rect.getSize().x / 2)
#define MID_POS_Y (m_Rect.getPosition().y + m_Rect.getSize().y / 2)
#define TEXT_POS_X(text) (MID_POS_X - text.getLocalBounds().width / 2)
#define TEXT_POS_Y(text) (MID_POS_Y - text.getLocalBounds().height / 2)
#define CURSOR_POS_X (MID_POS_X + m_Text.getLocalBounds().width / 2)
#define CURSOR_POS_Y (MID_POS_Y - m_Text.getLocalBounds().height / 2)

TextInput::TextInput(float x, float y, float width, float height, const std::string& defaultText) {
	m_Rect.setPosition(x, y);
	m_Rect.setSize({ width, height });
	m_Rect.setFillColor(sf::Color::White);

	m_Cursor.setPosition(CURSOR_POS_X, CURSOR_POS_Y - 8);
	m_Cursor.setSize({ 2, 18 });
	m_Cursor.setFillColor(sf::Color::Black);

	m_Text.setFont(FontManager::GetFont());
	m_Text.setString(m_Input);
	m_Text.setCharacterSize(18);
	m_Text.setPosition(TEXT_POS_X(m_Text), TEXT_POS_Y(m_Text));
	m_Text.setFillColor(sf::Color::Black);

	m_DefaultText.setFont(FontManager::GetFont());
	m_DefaultText.setString(defaultText);
	m_DefaultText.setCharacterSize(18);
	m_DefaultText.setPosition(TEXT_POS_X(m_DefaultText), TEXT_POS_Y(m_DefaultText));
	m_DefaultText.setFillColor(sf::Color(200, 200, 200));
	m_DefaultText.setStyle(sf::Text::Italic);
}

void TextInput::OnUpdate(float deltaTime) {
	if (m_Clock.getElapsedTime().asSeconds() > m_BlinkingSeconds) {
		m_IsBlinking = !m_IsBlinking;
		m_Clock.restart();
	}
}

void TextInput::OnDraw(sf::RenderWindow& window) {
	window.draw(m_Rect);
	window.draw(m_Text);

	if (m_TakeInput && m_IsBlinking)
		window.draw(m_Cursor);

	if (!m_TakeInput && m_Input.empty())
		window.draw(m_DefaultText);
}

void TextInput::OnEvent(const sf::Event& event) {
	if (m_TakeInput) {
		if (event.type == sf::Event::TextEntered) {
			// Backspace (i couldnt found a const for backspace unicode)
			if (event.text.unicode == 8) {
				if (!m_Input.empty()) {
					m_Input.pop_back();
					m_Text.setString(m_Input);
					m_Text.setPosition(TEXT_POS_X(m_Text), TEXT_POS_Y(m_Text));
					m_Cursor.setPosition(CURSOR_POS_X, CURSOR_POS_Y);
				}
			}
			else if (event.text.unicode < 128) {
				m_Input += (char)(event.text.unicode);
				m_Text.setString(m_Input);
				m_Text.setPosition(TEXT_POS_X(m_Text), TEXT_POS_Y(m_Text));
				m_Cursor.setPosition(CURSOR_POS_X, CURSOR_POS_Y);
			}
		}
	}
	if (event.type == sf::Event::MouseButtonPressed)
		m_TakeInput = isMouseOn();
}

bool TextInput::isMouseOn()
{
	const sf::WindowBase& windowBase = Application::Get()->GetWindowBase();

	return (float)sf::Mouse::getPosition(windowBase).x > m_Rect.getPosition().x &&
			(float)sf::Mouse::getPosition(windowBase).x < m_Rect.getPosition().x + m_Rect.getSize().x &&
			(float)sf::Mouse::getPosition(windowBase).y > m_Rect.getPosition().y &&
			(float)sf::Mouse::getPosition(windowBase).y < m_Rect.getPosition().y + m_Rect.getSize().y;
}
