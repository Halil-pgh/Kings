#include "pch.h"
#include "ShopBar.h"

#include "UI/FontManager.h"
#include "Core/Application.h"
#include "Core/TextureManager.h"
#include "Entities/Buildings/Building.h"

#define CUBE_SIZE 40.0f
#define FIRST_X (((float)Application::Get()->GetWindowBase().getSize().x - CUBE_SIZE * (int)BuildingType::Count) / 2.0f)
#define FIRST_Y (6.0f * (float)Application::Get()->GetWindowBase().getSize().y / 7.0f)

ShopBar::ShopBar() {
	// this needs to be synced with BuildingType
	m_Keys = {'Q', 'W'};

	m_Rects.reserve((int)BuildingType::Count);
	m_KeysText.reserve((int)BuildingType::Count);
	for (uint32_t i = 0; i < (int)BuildingType::Count; i++) {
		// Rectangles
		sf::RectangleShape rect({CUBE_SIZE, CUBE_SIZE});
		rect.setOutlineThickness(5.0f);
		rect.setOutlineColor(sf::Color(211, 214, 208));
		rect.setPosition(FIRST_X + (float)i * (CUBE_SIZE + rect.getOutlineThickness()), FIRST_Y);
		rect.setFillColor(sf::Color(255, 255, 255, 200));
		switch (i) {
			case (uint32_t)BuildingType::Home: {
				rect.setTexture(TextureManager::GetTexture("home"));
				break;
			}
			case (uint32_t)BuildingType::Mine: {
				rect.setTexture(TextureManager::GetTexture("mine"));
				break;
			}
			default: {
				std::cout << "Probably forget to implement a item texture :)\n";
				break;
			}
		}
		m_Rects.push_back(rect);

		// Keyboard
		sf::Text text;
		text.setFont(FontManager::GetFont("normal"));
		text.setCharacterSize(12);
		text.setString(m_Keys[i]);
		text.setPosition(rect.getPosition());
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Regular);
		m_KeysText.push_back(text);
	}
}

void ShopBar::OnUpdate(float deltaTime) {
}

void ShopBar::OnDraw(sf::RenderWindow &window) {
	for (const auto& rect : m_Rects) {
		window.draw(rect);
	}
	for (const auto& text : m_KeysText) {
		window.draw(text);
	}
}

bool ShopBar::OnEvent(const sf::Event &event) {
	return false;
}
