#include "pch.h"
#include "ServerList.h"

#include "Core/Application.h"
#include "Core/SceneManager.h"

#define WINDOW_SIZE (float)Application::Get()->GetWindowBase().getSize()
#define ROWS 5
#define COLUMNS 3
#define LIST_PADDING 20
#define SERVER_CART_HEIGHT 100

ServerList::ServerList()
	: m_BackButton(WINDOW_SIZE.x / (COLUMNS * 4), WINDOW_SIZE.y - 3 * (WINDOW_SIZE.y / (ROWS * 4)),
		WINDOW_SIZE.x / (COLUMNS * 2), WINDOW_SIZE.y / (ROWS * 2), "Back"),
	m_RefreshButton(5 * WINDOW_SIZE.x / (COLUMNS * 4), WINDOW_SIZE.y - 3 * (WINDOW_SIZE.y / (ROWS * 4)),
		WINDOW_SIZE.x / (COLUMNS * 2), WINDOW_SIZE.y / (ROWS * 2), "Refresh"),
	m_JoinButton(9 * WINDOW_SIZE.x / (COLUMNS * 4), WINDOW_SIZE.y - 3 * (WINDOW_SIZE.y / (ROWS * 4)),
		WINDOW_SIZE.x / (COLUMNS * 2), WINDOW_SIZE.y / (ROWS * 2), "Join") {
	m_JoinButton.SetActive(false);
	m_JoinButton.SetOnClickCallback([&]() {
		m_Client->JoinServer(m_SelectedServerInfo);
		SceneManager::SetActiveScene("Game");
	});
}

void ServerList::SetBackScene(const std::string& name) {
	m_BackButton.SetOnClickCallback([&]() {
		m_Client->ShoutDown();
		SceneManager::SetActiveScene(name);
	});
}

void ServerList::SetClient(Client* client) {
	m_Client = client;
	m_Client->RefreshServers();
	m_RefreshButton.SetOnClickCallback([&]() {
		m_Client->RefreshServers();
	});
}

void ServerList::OnUpdate(float deltaTime) {
	for (int i = 0; i < m_Client->m_ServerInfos.size(); i++) {
		bool here = false;
		for (ServerInfo& knownInfo : m_ServerInfos) {
			if (m_Client->m_ServerInfos[i] == knownInfo) {
				here = true;
				break;
			}
		}

		if (!here) {
			m_ServerInfos.push_back(m_Client->m_ServerInfos[i]);
			auto fi = (float)i;
			Button button = {
				(float)WINDOW_SIZE.x / 4, LIST_PADDING + fi * (LIST_PADDING + SERVER_CART_HEIGHT),
				(float)WINDOW_SIZE.x / 2, SERVER_CART_HEIGHT, "Name: " + m_Client->m_ServerInfos[i].name + ", Player Count: " + std::to_string(m_Client->m_ServerInfos[i].playerCount)
			};
			button.SetOnClickCallback([i, this]() {
				m_JoinButton.SetActive(true);
				m_SelectedServerInfo = m_Client->m_ServerInfos[i];
			});
			m_ServerButtons.push_back(button);
		}
	}

	m_BackButton.OnUpdate(deltaTime);
	m_RefreshButton.OnUpdate(deltaTime);
	m_JoinButton.OnUpdate(deltaTime);

	for (Button& button : m_ServerButtons)
		button.OnUpdate(deltaTime);
}

void ServerList::OnDraw(sf::RenderWindow& window)
{
	m_BackButton.OnDraw(window);
	m_RefreshButton.OnDraw(window);
	m_JoinButton.OnDraw(window);

	for (Button& button : m_ServerButtons)
		button.OnDraw(window);
}

void ServerList::OnEvent(const sf::Event& event)
{
	m_BackButton.OnEvent(event);
	m_RefreshButton.OnEvent(event);
	m_JoinButton.OnEvent(event);

	for (Button& button : m_ServerButtons)
		button.OnEvent(event);
}
