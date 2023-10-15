#include "pch.h"
#include "Self.h"

#include "Networking/Server.h"
#include "Networking/Client.h"
#include "Core/SceneManager.h"
#include "Core/Application.h"

Self::Self()
	: m_Speed(100), m_Networker(nullptr)
{
}

Self::~Self()
{
	if (m_Networker)
		delete m_Networker;
}

void Self::OnUpdate(float deltaTime)
{
	m_Text.move(m_Velocity * deltaTime);
	m_Rect.move(m_Velocity * deltaTime);
	Application::Get()->GetCamera().setCenter(m_Rect.getPosition() + sf::Vector2f{m_Rect.getSize().x / 2, m_Rect.getSize().y / 2});

	sf::Vector2i mousePos = sf::Mouse::getPosition(Application::Get()->GetWindowBase());
	float xdiff = mousePos.x - m_Rect.getPosition().x;
	float ydiff = mousePos.y - m_Rect.getPosition().y;
	float radian = atan2f(ydiff, xdiff);
	static const double myPi = 3.14159265359;
	m_Rect.setRotation(radian * (180.0f / myPi));

	float line = sqrt(pow(xdiff, 2) + pow(ydiff, 2));
	float times = line / m_Speed;
	if (times != 0.0f)
	{
		m_Velocity.x = xdiff / times;
		m_Velocity.y = ydiff / times;
	}

	HandleConnection();
}

void Self::OnEvent(const sf::Event& event)
{
}

void Self::BecomeServer(const std::string& serverName)
{
	m_Networker = new Server(serverName);
	m_Networker->SetPlayerData({
		m_Networker->GetUUID(),
		m_Name,
		m_Rect.getFillColor(),
		m_Rect.getPosition()
	});
	m_JoinedUUIDs.push_back(m_Networker->GetUUID());
	m_Networker->Run();
}

void Self::BecomeClient()
{
	m_Networker = new Client();
	m_Networker->SetPlayerData({
		m_Networker->GetUUID(),
		m_Name,
		m_Rect.getFillColor(),
		m_Rect.getPosition()
	});
	m_JoinedUUIDs.push_back(m_Networker->GetUUID());
	m_Networker->Run();
}

Client* Self::GetClient()
{
	auto client = dynamic_cast<Client*>(m_Networker);
	if (client)
		return client;
	std::cout << "Client is null!\n";
	return nullptr;
}

Server* Self::GetServer()
{
	auto server = dynamic_cast<Server*>(m_Networker);
	if (server)
		return server;
	std::cout << "Server is null!\n";
	return nullptr;
}

void Self::HandleConnection()
{
	m_Networker->SetPlayerData({
		m_Networker->GetUUID(),
		m_Name,
		m_Rect.getFillColor(),
		m_Rect.getPosition(),
		m_Rect.getRotation()
	});

	for (const PlayerData& player : m_Networker->GetPlayers())
	{
		bool already = false;
		for (int i = 0; i < m_JoinedUUIDs.size(); i++)
		{
			if (m_JoinedUUIDs[i] == player.uuid)
			{
				if (i > 0)
					m_OtherPlayers[i - 1]->Reload(player);
				already = true;
			}
		}

		if (already)
			continue;

		Player* newPlayer = new Player();
		newPlayer->Reload(player);

		SceneManager::GetActiveScene()->AddEntity(newPlayer);
		m_JoinedUUIDs.push_back(player.uuid);
		m_OtherPlayers.push_back(newPlayer);
	}
}
