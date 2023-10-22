#include "pch.h"
#include "Self.h"

#include "Entities/Buildings/Home.h"
#include "Entities/Buildings/Mine.h"
#include "Networking/Server.h"
#include "Networking/Client.h"
#include "Core/SceneManager.h"
#include "Core/Application.h"

Self::Self()
	: m_Speed(100), m_Networker(nullptr) {
}

Self::~Self() {
	delete m_Networker;
}

void Self::OnUpdate(float deltaTime) {
	m_Text.move(m_Velocity * deltaTime);
	m_Rect.move(m_Velocity * deltaTime);
	Application::Get()->GetCamera().setCenter(m_Rect.getPosition() + sf::Vector2f{m_Rect.getSize().x / 2, m_Rect.getSize().y / 2});

	FollowMouse();
	HandleConnection();
}

void Self::OnEvent(const sf::Event& event) {
	switch (event.type) {
		case sf::Event::KeyPressed: {
			switch (event.key.code) {
				case sf::Keyboard::Q: {
					auto home = new Home(m_Rect.getPosition().x, m_Rect.getPosition().y);
					m_Buildings.push_back(home);
					break;
				}
				case sf::Keyboard::W: {
					auto mine = new Mine(m_Rect.getPosition().x, m_Rect.getPosition().y);
					m_Buildings.push_back(mine);
					break;
				}
				default: {
					break;
				}
			}
			break;
		}
		default: {
			break;
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

		SceneManager::GetActiveScene()->AddEntity(newPlayer);
		m_JoinedUUIDs.push_back(player.uuid);
		m_OtherPlayers.push_back(newPlayer);
	}
}

void Self::FollowMouse() {
	// Calculating mouse positing
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(Application::Get()->GetWindowBase()));
	mousePos.x += Application::Get()->GetCamera().getCenter().x -
			Application::Get()->GetCamera().getSize().x / 2;
	mousePos.y += Application::Get()->GetCamera().getCenter().y -
			Application::Get()->GetCamera().getSize().y / 2;

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
	   m_Rect.getRotation()
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
	   m_Rect.getRotation()
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
