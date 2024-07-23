#include "pch.h"
#include "ServerAdder.h"

#include "Core/Application.h"
#include "Core/SceneManager.h"

#define WINDOW_SIZE (float)Application::Get()->GetWindowBase().getSize()
#define ROWS 7
#define COLUMNS 6
#define PADDING 30

ServerAdder::ServerAdder() {

	m_NameInput = std::make_shared<TextInput>(2.5 * WINDOW_SIZE.x / COLUMNS, 9 * WINDOW_SIZE.y / (4 * ROWS) - PADDING,
											  WINDOW_SIZE.x / COLUMNS, WINDOW_SIZE.y / (2 * ROWS), "Name");
	m_IPInput = std::make_shared<TextInput>(2.5 * WINDOW_SIZE.x / COLUMNS, 13 * WINDOW_SIZE.y / (4 * ROWS) - PADDING,
											WINDOW_SIZE.x / COLUMNS, WINDOW_SIZE.y / (2 * ROWS), "IP");
	m_AddButton = std::make_shared<Button>(3.5 * WINDOW_SIZE.x / COLUMNS, 17 * WINDOW_SIZE.y / (4 * ROWS) - PADDING,
										   WINDOW_SIZE.x / (2 * COLUMNS), WINDOW_SIZE.y / (2 * ROWS), "Add");
	m_BackButton = std::make_shared<Button>(2 * WINDOW_SIZE.x / COLUMNS, 17 * WINDOW_SIZE.y / (4 * ROWS) - PADDING,
											WINDOW_SIZE.x / (2 * COLUMNS), WINDOW_SIZE.y / (2 * ROWS), "Back");

	Application::Get()->SetButtonTheme(m_AddButton);
	Application::Get()->SetButtonTheme(m_BackButton);

	m_AddButton->SetOnClickCallback([&]() {
		m_OnServerAdd(m_NameInput->Get(), m_IPInput->Get());
		SceneManager::GetActiveScene()->RemoveLayer("AddServer");
	});

	m_BackButton->SetOnClickCallback([&]() {
		SceneManager::GetActiveScene()->RemoveLayer("AddServer");
	});
}

void ServerAdder::OnUpdate(float deltaTime) {
	m_AddButton->OnUpdate(deltaTime);
	m_BackButton->OnUpdate(deltaTime);
	m_NameInput->OnUpdate(deltaTime);
	m_IPInput->OnUpdate(deltaTime);
}

void ServerAdder::OnDraw(sf::RenderWindow &window) {
	window.draw(m_Rect);
	m_AddButton->OnDraw(window);
	m_BackButton->OnDraw(window);
	m_NameInput->OnDraw(window);
	m_IPInput->OnDraw(window);
}

bool ServerAdder::OnEvent(const sf::Event &event) {
	if (m_AddButton->OnEvent(event)) return true;
	if (m_BackButton->OnEvent(event)) return true;
	if (m_NameInput->OnEvent(event)) return true;
	if (m_IPInput->OnEvent(event)) return true;
	return true;
}

void ServerAdder::OnAttach() {
	m_Rect.setPosition(WINDOW_SIZE.x / 4, WINDOW_SIZE.y / 4 - PADDING);
	m_Rect.setSize({WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 2});
	m_Rect.setFillColor(sf::Color(15, 148, 92, 50));
}
