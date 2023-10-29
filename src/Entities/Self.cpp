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
	: m_Speed(100), m_Networker(nullptr), m_PlayerStats(new PlayerStats()), m_ShopBar(new ShopBar()) {
}

Self::~Self() {
	delete m_Networker;
}

void Self::OnAttach() {
	SceneManager::GetScene("Game")->GetLayer("UI")->AddEntity(m_PlayerStats);
	SceneManager::GetScene("Game")->GetLayer("UI")->AddEntity(m_ShopBar);
}

void Self::OnUpdate(float deltaTime) {
	sf::View& camera = SceneManager::GetActiveScene()->GetLayer("Game")->GetView();
	camera.setCenter((sf::Vector2f)sf::Vector2i(m_Rect.getPosition()));

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
	window.clear(sf::Color(82, 115, 6));

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
	if (auto client = dynamic_cast<Client*>(m_Networker); client != nullptr) {
		if (!client->IsInGame()) {
			SceneManager::SetActiveScene("Main");
			Application::Get()->DestroyGameScene();
			return true;
		}
	}

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
				case sf::Keyboard::Escape: {
					if (m_Mode == Mode::Build) {
						m_Mode = Mode::Walk;
					}
					else {
						m_Mode = Mode::Settings;
						SceneManager::GetActiveScene()->AddLayer("Menu", 1);
						auto options = new Options();
						options->OnDetach([&]() {
							m_Mode = Mode::Walk;
						});
						options->OnDisconnect([&]() {
							SceneManager::SetActiveScene("Main");
							Application::Get()->DestroyGameScene();
						});
						SceneManager::GetActiveScene()->GetLayer("Menu")->AddEntity(options);
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
						if (m_ProductionBuilding->GetType() == BuildingType::Home) {
							auto newHome = new Home(*(Home*)m_ProductionBuilding.get());
							m_MaxSoldier += newHome->GetMaxCount();
							m_PlayerStats->SetMaxSoldierCount(m_MaxSoldier);
							newBuilding = newHome;
						}
						else if (m_ProductionBuilding->GetType() == BuildingType::Mine) {
							auto newMine = new Mine(*(Mine*)m_ProductionBuilding.get());
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
		buildingData.push_back({
			building->GetType(),
			building->GetPosition()
		});
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
		for (int i = 0; i < m_JoinedUUIDs.size(); i++) {
			if (m_JoinedUUIDs[i] == player.uuid) {
				if (i > 0)
					m_OtherPlayers[i - 1]->Reload(player);
				break;
			}
		}
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
	InitNetworker();
}

void Self::BecomeClient() {
	// Delete/clear everything before becoming client!
	delete m_Networker;
	m_JoinedUUIDs.clear();

	m_Networker = new Client();
	InitNetworker();
}

Client** Self::GetClient() {
	return reinterpret_cast<Client **>(&m_Networker);
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

void Self::InitNetworker() {
	m_Networker->SetPlayerData({
		m_Networker->GetUUID(),
		m_Name,
		m_Rect.getFillColor(),
		m_Rect.getPosition(),
		m_Rect.getRotation(),
		{}
	});
	m_Networker->OnConnect([&](const PlayerData& player) {
		auto newPlayer = new Player();
		newPlayer->Reload(player);
		std::cout << "Someone new !\n";
		std::cout << player.uuid << "\n";

		SceneManager::GetActiveScene()->GetLayer("Game")->AddEntity(newPlayer);
		m_JoinedUUIDs.push_back(player.uuid);
		m_OtherPlayers.push_back(newPlayer);
	});

	m_Networker->OnDisconnect([&](uint64_t uuid) {
		uint32_t i = 0;
		for (auto player : m_OtherPlayers) {
			if (player->GetUUID() == uuid) {
				SceneManager::GetActiveScene()->GetLayer("Game")->RemoveEntity(player);
				m_OtherPlayers.erase(m_OtherPlayers.begin() + i);
				m_JoinedUUIDs.erase(m_JoinedUUIDs.begin() + i + 1);
				break;
			}
			i++;
		}
	});
	m_JoinedUUIDs.push_back(m_Networker->GetUUID());
	m_Networker->Run();
}
