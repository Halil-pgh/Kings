#include "pch.h"

#include "Player.h"
#include "UI/FontManager.h"
#include "Core/Random.h"
#include "Entities/Buildings/Home.h"
#include "Entities/Buildings/Mine.h"

#define TEXT_POS_X (m_Rect.getPosition().x - m_Text.getLocalBounds().width / 2)
#define TEXT_POS_Y (m_Rect.getPosition().y - (m_Rect.getSize().y / 2) - m_Text.getLocalBounds().height * 1.5)

Player::Player()
	: m_Velocity(0, 0) {
	m_Rect.setPosition(100, 100);
	m_Rect.setSize({ 50, 50 });
	m_Rect.setOrigin(m_Rect.getSize().x / 2, m_Rect.getSize().y / 2);
	m_Rect.setFillColor(sf::Color(Random::GenerateInt(0, 256), Random::GenerateInt(0, 256), Random::GenerateInt(0, 256)));

	m_Text.setFont(FontManager::GetFont());
	m_Text.setString("None");
	m_Text.setCharacterSize(18);
	m_Text.setPosition(TEXT_POS_X, TEXT_POS_Y);
}

void Player::OnDraw(sf::RenderWindow& window) {
	window.draw(m_Rect);
	window.draw(m_Text);

    for (auto building : m_Buildings) {
        building->OnDraw(window);
    }
}

void Player::SetName(const std::string& name) {
	m_Name = name;
	m_Text.setString(name);
	m_Text.setPosition(TEXT_POS_X, TEXT_POS_Y);
}

void Player::Reload(const PlayerData& data) {
	m_Rect.setFillColor(data.color);
	m_Rect.setRotation(data.rotation);
	m_Rect.setPosition(data.position);
	m_Text.setPosition(TEXT_POS_X, TEXT_POS_Y);
	m_Text.setString(data.name);

    // TODO: pls find a better way around :/
    for (auto building : m_Buildings) {
        delete building;
    }
    m_Buildings.clear();

    for (const auto& building : data.buildings) {
        switch (building.type) {
            case BuildingType::Home: {
                m_Buildings.push_back(new Home(building.position.x, building.position.y));
            }
            case BuildingType::Mine: {
                m_Buildings.push_back(new Mine(building.position.x, building.position.y));
            }
        }
    }
}
