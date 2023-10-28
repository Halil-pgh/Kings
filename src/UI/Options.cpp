#include "pch.h"
#include "Options.h"

#include "Core/SceneManager.h"
#include "FontManager.h"

Options::Options() {
	m_Heading.setFont(FontManager::GetFont("normal"));
	m_Heading.setCharacterSize(36);
	m_Heading.setString("Options");
	m_Heading.setPosition(10, 10);
	m_Heading.setFillColor(sf::Color::Black);
	m_Heading.setStyle(sf::Text::Bold);

	m_Text.setFont(FontManager::GetFont("normal"));
	m_Text.setCharacterSize(24);
	m_Text.setString("Esc to exit :)");
	m_Text.setPosition(10, 100);
	m_Text.setFillColor(sf::Color::Black);
	m_Text.setStyle(sf::Text::Regular);
}

void Options::OnAttach() {
	sf::View& view = SceneManager::GetScene("Game")->GetLayer("Menu")->GetView();
	view.setViewport(sf::FloatRect(0.25f, 0.25f, 0.5f, 0.5f));

	m_Rect.setPosition(0, 0);
	m_Rect.setSize(view.getSize());
	m_Rect.setFillColor(sf::Color(100, 100, 100));
}

void Options::OnUpdate(float deltaTime) {
}

void Options::OnDraw(sf::RenderWindow &window) {
	window.draw(m_Rect);
	window.draw(m_Heading);
	window.draw(m_Text);
}

bool Options::OnEvent(const sf::Event &event) {
	switch (event.type) {
		case sf::Event::KeyPressed: {
			switch (event.key.code) {
				case sf::Keyboard::Escape: {
					m_OnDetach();
					SceneManager::GetActiveScene()->RemoveLayer("Menu");
					return true;
				}
				default: {
					return true;
				}
			}
		}
		default: {
			return false;
		}
	}
}
