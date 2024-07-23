#include "pch.h"
#include "ServerList.h"

#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "ServerAdder.h"

#define WINDOW_SIZE (float)Application::Get()->GetWindowBase().getSize()
#define ROWS 5
#define COLUMNS 3
#define LIST_PADDING 20
#define SERVER_CART_HEIGHT 100

ServerList::ServerList() {
	m_BackButton = std::make_shared<Button>(0.5 * WINDOW_SIZE.x / (COLUMNS * 4), WINDOW_SIZE.y - 3 * (WINDOW_SIZE.y / (ROWS * 4)),
											WINDOW_SIZE.x / (COLUMNS * 2), WINDOW_SIZE.y / (ROWS * 2), "Back");
	m_RefreshButton = std::make_shared<Button>(3.5 * WINDOW_SIZE.x / (COLUMNS * 4), WINDOW_SIZE.y - 3 * (WINDOW_SIZE.y / (ROWS * 4)),
											   WINDOW_SIZE.x / (COLUMNS * 2), WINDOW_SIZE.y / (ROWS * 2), "Refresh");
	m_JoinButton = std::make_shared<Button>(6.5 * WINDOW_SIZE.x / (COLUMNS * 4), WINDOW_SIZE.y - 3 * (WINDOW_SIZE.y / (ROWS * 4)),
											WINDOW_SIZE.x / (COLUMNS * 2), WINDOW_SIZE.y / (ROWS * 2), "Join");
	m_AddServerButton = std::make_shared<Button>(9.5 * WINDOW_SIZE.x / (COLUMNS * 4), WINDOW_SIZE.y - 3 * (WINDOW_SIZE.y / (ROWS * 4)),
												 WINDOW_SIZE.x / (COLUMNS * 2), WINDOW_SIZE.y / (ROWS * 2), "Add Server");
	m_JoinButton->SetActive(false);
	m_JoinButton->SetOnClickCallback([&]() {
		m_Client->connect(m_SelectedServerInfo.ip.c_str(), Server::PORT);
		if (SceneManager::GetScene("Game") == nullptr)
			Application::Get()->CreateGameScene();
		SceneManager::SetActiveScene("Game");
		Application::Get()->DestroyServerList();
	});
	m_AddServerButton->SetOnClickCallback([&]() {
		SceneManager::GetActiveScene()->AddLayer("AddServer", 1);
		auto serverAdder = std::make_shared<ServerAdder>();
		SceneManager::GetActiveScene()->GetLayer("AddServer")->AddEntity(serverAdder);
		serverAdder->OnServerAdd([&](const std::string& name, const std::string& ip) {
			m_ServerInfos.push_back({name, ip});
			std::shared_ptr<Button> button = std::make_shared<Button>(
				(float)WINDOW_SIZE.x / 4, LIST_PADDING + ((float)m_ServerInfos.size() - 1) * (LIST_PADDING + SERVER_CART_HEIGHT),
				(float)WINDOW_SIZE.x / 2, SERVER_CART_HEIGHT, "Name: " + name + ", IP: " + ip
			);
			button->SetOnClickCallback([&, name, ip]() {
				m_JoinButton->SetActive(true);
				m_SelectedServerInfo = {name, ip};
			});
			m_ServerButtons.push_back(button);
		});
	});

	Application::Get()->SetButtonTheme(m_BackButton);
	Application::Get()->SetButtonTheme(m_RefreshButton);
	Application::Get()->SetButtonTheme(m_JoinButton);
	Application::Get()->SetButtonTheme(m_AddServerButton);
}

void ServerList::SetBackScene(const std::string& name) {
	m_BackButton->SetOnClickCallback([&, name]() {
		m_JoinButton->SetActive(false);
		m_SelectedServerInfo = {};

		SceneManager::SetActiveScene(name);
		Application::Get()->DestroyServerList();
	});
}

void ServerList::SetClient(const std::shared_ptr<Client>& client) {
	m_Client = client;
	/*
	(*m_Client)->RefreshServers();
	m_RefreshButton->SetOnClickCallback([&]() {
		(*m_Client)->RefreshServers();
	});
	 */
}

void ServerList::OnUpdate(float deltaTime) {
	m_BackButton->OnUpdate(deltaTime);
	m_RefreshButton->OnUpdate(deltaTime);
	m_JoinButton->OnUpdate(deltaTime);
	m_AddServerButton->OnUpdate(deltaTime);

	for (std::shared_ptr<Button>& button : m_ServerButtons)
		button->OnUpdate(deltaTime);
}

void ServerList::OnDraw(sf::RenderWindow& window) {
	m_BackButton->OnDraw(window);
	m_RefreshButton->OnDraw(window);
	m_JoinButton->OnDraw(window);
	m_AddServerButton->OnDraw(window);

	for (std::shared_ptr<Button>& button : m_ServerButtons)
		button->OnDraw(window);
}

bool ServerList::OnEvent(const sf::Event& event) {
	if (m_BackButton->OnEvent(event)) return true;
	if (m_RefreshButton->OnEvent(event)) return true;
	if (m_JoinButton->OnEvent(event)) return true;
	if (m_AddServerButton->OnEvent(event)) return true;

	for (std::shared_ptr<Button>& button : m_ServerButtons) {
		if (button->OnEvent(event)) {
			return true;
		}
	}
	return false;
}
