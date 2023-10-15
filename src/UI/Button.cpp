#include "pch.h"
#include "Button.h"

#include "Core/Application.h"
#include "FontManager.h"

#define CHAR_WIDTH 5
#define CHAR_HEIGHT 10

Button::Button(float x, float y, float width, float height, const std::string& mess)
	: m_NormalColor(255, 255, 255), m_HighlightedColor(200, 200, 200), m_PressedColor(150, 150, 150),
	m_OnClick([]() {})
{
	m_Rect.setPosition(x, y);
	m_Rect.setSize({ width, height });

	m_Text.setFont(FontManager::GetFont());
	// TODO: use m_Text.getLocalBounds().width | height
	m_Text.setPosition(x + width / 2 - (float)mess.length() * CHAR_WIDTH, y + height / 2 - CHAR_HEIGHT);
	m_Text.setString(mess);
	m_Text.setCharacterSize(18);
	m_Text.setFillColor(sf::Color::Black);
}

void Button::OnUpdate(float deltaTime)
{
	// Return hell to not use 'else'
	// i think it is wrong but anyway
	if (!m_Active)
	{
		// We need a variable for that!
		m_Rect.setFillColor(sf::Color(100, 100, 100));
		return;
	}

	if (isMouseOn())
	{
		if (m_Rect.getFillColor() == m_PressedColor)
			return;

		m_Rect.setFillColor(m_HighlightedColor);
		return;
	}
	m_Rect.setFillColor(m_NormalColor);
}

void Button::OnDraw(sf::RenderWindow& window)
{
	window.draw(m_Rect);
	window.draw(m_Text);
}

void Button::OnEvent(const sf::Event& event)
{
	if (!m_Active)
		return;

	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left && isMouseOn())
			{
				m_Rect.setFillColor(m_PressedColor);
				m_OnClick();
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (isMouseOn())
					m_Rect.setFillColor(m_HighlightedColor);
				else
					m_Rect.setFillColor(m_NormalColor);
			}
			break;
		}
        default:
            break;
	}
}

bool Button::isMouseOn()
{
	const sf::WindowBase& windowBase = Application::Get()->GetWindowBase();

	return (float)sf::Mouse::getPosition(windowBase).x > m_Rect.getPosition().x &&
            (float)sf::Mouse::getPosition(windowBase).x < m_Rect.getPosition().x + m_Rect.getSize().x &&
            (float)sf::Mouse::getPosition(windowBase).y > m_Rect.getPosition().y &&
            (float)sf::Mouse::getPosition(windowBase).y < m_Rect.getPosition().y + m_Rect.getSize().y;
}
