#include "pch.h"
#include "PlayerStats.h"

#include "Core/Application.h"
#include "FontManager.h"

#define WINDOW_SIZE (float)Application::Get()->GetWindowBase().getSize()

PlayerStats::PlayerStats() {
	m_MoneyText.setFont(FontManager::GetFont("normal"));
	m_MoneyText.setCharacterSize(24);
	m_MoneyText.setString("0 $");
	m_MoneyText.setPosition(0, 0);
	m_MoneyText.setFillColor(sf::Color::White);
	m_MoneyText.setStyle(sf::Text::Regular);

	m_MaxSoldierText.setFont(FontManager::GetFont("normal"));
	m_MaxSoldierText.setCharacterSize(24);
	m_MaxSoldierText.setString("0/0");
	m_MaxSoldierText.setPosition(WINDOW_SIZE.x - m_MaxSoldierText.getGlobalBounds().width - 50, 0);
	m_MaxSoldierText.setFillColor(sf::Color::White);
	m_MaxSoldierText.setStyle(sf::Text::Regular);
}

void PlayerStats::OnUpdate(float deltaTime) {
}

void PlayerStats::OnDraw(sf::RenderWindow &window) {
	window.draw(m_MoneyText);
	window.draw(m_MaxSoldierText);
}

bool PlayerStats::OnEvent(const sf::Event &event) {
	return false;
}

void PlayerStats::SetMoney(uint32_t money) {
	m_MoneyText.setString(std::to_string(money) + " $");
}

void PlayerStats::SetMaxSoldierCount(uint32_t count) {
	m_MaxSoldierText.setString("0/" + std::to_string(count));
}
