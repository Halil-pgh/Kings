#include "pch.h"
#include "Self.h"

#include "Entities/Buildings/Home.h"
#include "Entities/Buildings/Mine.h"
#include "Networking/Server.h"
#include "Networking/Client.h"
#include "Core/SceneManager.h"
#include "Core/Application.h"
#include "UI/FontManager.h"
#include "UI/Options.h"

#define WINDOW_SIZE (float)Application::Get()->GetWindowBase().getSize()

Self::Self()
	: m_Speed(100), m_Networker(nullptr), m_PlayerStats(new PlayerStats()) {
}

Self::~Self() {
	delete m_Networker;
}

void Self::OnAttach() {
	SceneManager::GetScene("Game")->GetLayer("UI")->AddEntity(m_PlayerStats);
}

void Self::OnUpdate(float deltaTime) {
	sf::View& camera = SceneManager::GetActiveScene()->GetLayer("Game")->GetView();
	camera.setCenter((sf::Vector2f)sf::Vector2i(m_Rect.getPosition() + sf::Vector2f{m_Rect.getSize().x / 2, m_Rect.getSize().y / 2}));

	if (m_Mode == Mode::Walk) {
		m_Text.move(m_Velocity * deltaTime);
		m_Rect.move(m_Velocity * deltaTime);
	}
	else if (m_Mode == Mode::Build) {
		sf::Vector2f mousePos = Application::GetMousePosition(SceneManager::GetActiveScene()->GetLayer("Game")->GetView());
		m_ProductionBuilding->SetPosition(mousePos);
	}

	// Updating money $$$
	m_MoneyTime += deltaTime;
	if (m_MoneyTime >= 1) {
		m_Money += m_MoneyPerSecond;
		m_PlayerStats->SetMoney(m_Money);
		m_MoneyTime = 0;
	}

	FollowMouse();
	HandleConnection();
}

void Self::OnDraw(sf::RenderWindow &window) {
	window.draw(m_Rect);
	window.draw(m_Text);

	for (auto building : m_Buildings) {
		building->OnDraw(window);
	}

	if (m_Mode == Mode::Build) {
		m_ProductionBuilding->SetProductable(CheckBuildingsForProduction());
		m_ProductionBuilding->OnDraw(window);
	}
}

bool Self::OnEvent(const sf::Event& event) {
	switch (event.type) {
		case sf::Event::KeyPressed: {
			sf::Vector2f mousePos = Application::GetMousePosition(SceneManager::GetActiveScene()->GetLayer("Game")->GetView());
			switch (event.key.code) {
				case sf::Keyboard::Q: {
					m_Mode = Mode::Build;
					m_ProductionBuilding = std::make_unique<Home>(mousePos.x, mousePos.y);
					m_ProductionBuilding->SetProduction(true);
					return true;
				}
				case sf::Keyboard::W: {
					m_Mode = Mode::Build;
					m_ProductionBuilding = std::make_unique<Mine>(mousePos.x, mousePos.y);
					m_ProductionBuilding->SetProduction(true);
					return true;
				}
				case sf::Keyboard::S: {
					SceneManager::GetActiveScene()->AddLayer("Menu", 1);
					auto options = new Options();
					SceneManager::GetActiveScene()->GetLayer("Menu")->AddEntity(options);
					return true;
				}
				case sf::Keyboard::Escape: {
					if (m_Mode == Mode::Build) {
						m_Mode = Mode::Walk;
					}
					return true;
				}
				default: {
					return false;
				}
			}
		}
		case sf::Event::MouseButtonPressed: {
			switch (event.mouseButton.button) {
				case sf::Mouse::Button::Left: {
					if (m_Mode == Mode::Build) {
						m_Mode = Mode::Walk;
						if (!CheckBuildingsForProduction()) {
							break;
						}

						Building* newBuilding;
						if (auto home = dynamic_cast<Home*>(m_ProductionBuilding.get()); home != nullptr) {
							auto newHome = new Home(*home);
							m_MaxSoldier += newHome->GetMaxCount();
							m_PlayerStats->SetMaxSoldierCount(m_MaxSoldier);
							newBuilding = newHome;
						}
						else if (auto mine = dynamic_cast<Mine*>(m_ProductionBuilding.get()); mine != nullptr) {
							auto newMine = new Mine(*mine);
							m_MoneyPerSecond += newMine->MoneyPerSecond();
							newBuilding = newMine;
						}
						newBuilding->SetProduction(false);
						m_Buildings.push_back(newBuilding);
						return true;
					}
					return false;
				}
				default: {
					return false;
				}
			}
		}
		default: {
			return false;
		}
	}
}

void Self::HandleConnection() {
	// TODO: I have to do that better way :/
	std::vector<BuildingData> buildingData(m_Buildings.size());
	for (auto building : m_Buildings) {
		if (dynamic_cast<Mine*>(building) != nullptr) {
			buildingData.push_back({
				BuildingType::Mine,
				building->GetPosition()
			});
		}
		else if (dynamic_cast<Home*>(building) != nullptr) {
			buildingData.push_back({
				BuildingType::Home,
				building->GetPosition()
			});
		}
	}

	m_Networker->SetPlayerData({
		m_Networker->GetUUID(),
		m_Name,
		m_Rect.getFillColor(),
		m_Rect.getPosition(),
		m_Rect.getRotation(),
		buildingData
	});

	for (const PlayerData& player : m_Networker->GetPlayers()) {
		bool already = false;
		for (int i = 0; i < m_JoinedUUIDs.size(); i++) {
			if (m_JoinedUUIDs[i] == player.uuid) {
				if (i > 0)
					m_OtherPlayers[i - 1]->Reload(player);
				already = true;
				break;
			}
		}

		if (already)
			continue;

		auto newPlayer = new Player();
		newPlayer->Reload(player);
		std::cout << "Someone new !\n";
		std::cout << player.uuid << "\n";

		SceneManager::GetActiveScene()->GetLayer("Game")->AddEntity(newPlayer);
		m_JoinedUUIDs.push_back(player.uuid);
		m_OtherPlayers.push_back(newPlayer);
	}
}

void Self::FollowMouse() {
	sf::Vector2f mousePos = Application::GetMousePosition(SceneManager::GetActiveScene()->GetLayer("Game")->GetView());
	float xdiff = mousePos.x - m_Rect.getPosition().x;
	float ydiff = mousePos.y - m_Rect.getPosition().y;
	float radian = atan2f(ydiff, xdiff);
	m_Rect.setRotation(radian * (180.0f / (float)std::numbers::pi));

	auto line = (float)sqrt(pow(xdiff, 2) + pow(ydiff, 2));
	float times = line / m_Speed;
	if (times != 0.0f) {
		m_Velocity.x = xdiff / times;
		m_Velocity.y = ydiff / times;
	}
}

void Self::BecomeServer(const std::string& serverName) {
	// Delete/clear everything before becoming server!
	delete m_Networker;
	m_JoinedUUIDs.clear();

	m_Networker = new Server(serverName);
	m_Networker->SetPlayerData({
	   m_Networker->GetUUID(),
	   m_Name,
	   m_Rect.getFillColor(),
	   m_Rect.getPosition(),
	   m_Rect.getRotation(),
	   {}
	});
	m_JoinedUUIDs.push_back(m_Networker->GetUUID());
	m_Networker->Run();
}

void Self::BecomeClient() {
	// Delete/clear everything before becoming client!
	delete m_Networker;
	m_JoinedUUIDs.clear();

	m_Networker = new Client();
	m_Networker->SetPlayerData({
	   m_Networker->GetUUID(),
	   m_Name,
	   m_Rect.getFillColor(),
	   m_Rect.getPosition(),
	   m_Rect.getRotation(),
	   {}
	});
	m_JoinedUUIDs.push_back(m_Networker->GetUUID());
	m_Networker->Run();
}

Client* Self::GetClient() {
	auto client = dynamic_cast<Client*>(m_Networker);
	if (client)
		return client;
	std::cout << "Client is null!\n";
	return nullptr;
}

Server* Self::GetServer() {
	auto server = dynamic_cast<Server*>(m_Networker);
	if (server)
		return server;
	std::cout << "Server is null!\n";
	return nullptr;
}

bool Self::CheckBuildingsForProduction() {
	bool result = std::ranges::all_of(m_Buildings, [&](auto building) {
		return !m_ProductionBuilding->GetRectangle().intersects(building->GetRectangle());
	});

	if (!result)
		return result;

	for (auto player : m_OtherPlayers) {
		result = std::ranges::all_of(player->m_Buildings, [&](auto building) {
			return !m_ProductionBuilding->GetRectangle().intersects(building->GetRectangle());
		});

		if (!result)
			return result;
	}

	return result;
}
