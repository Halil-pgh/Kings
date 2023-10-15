#include "pch.h"
#include "Application.h"

#include "Entities/Player.h"
#include "Entities/Self.h"
#include "UI/FontManager.h"
#include "UI/Button.h"
#include "UI/ServerList.h"
#include "UI/TextInput.h"
#include "SceneManager.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	m_Window.create(sf::VideoMode(1024, 640), "Game");
	m_Camera = m_Window.getDefaultView();
	SceneManager::Init();
	FontManager::SetFont("assets/fonts/arial.ttf");
}

Application::~Application()
{
	SceneManager::Destroy();
}

void Application::Run()
{
	Self* self = new Self();
	TextInput* textInput = new TextInput(GetWindowBase().getSize().x / 3, 150, GetWindowBase().getSize().x / 3, 50, "Enter your name");
	ServerList* serverList = new ServerList();

	Button* createButton = new Button(GetWindowBase().getSize().x / 6, 400, 200, 50, "Create Game");
	createButton->SetOnClickCallback([&]() {
		if (textInput->Get().empty())
			return;
		self->SetName(textInput->Get());
		self->BecomeServer(textInput->Get());

		SceneManager::SetActiveScene("Game");
	});

	Button* joinButton = new Button(7 * GetWindowBase().getSize().x / 12, 400, 200, 50, "Join Game");
	joinButton->SetOnClickCallback([&]() {
		if (textInput->Get().empty())
			return;
		self->SetName(textInput->Get());
		self->BecomeClient();
		serverList->SetClient(self->GetClient());
		serverList->SetBackScene("Main");

		SceneManager::SetActiveScene("Server List");
	});

	Scene* gameScene = new Scene("Game");
	gameScene->AddEntity(self);

	Scene* mainScene = new Scene("Main");
	mainScene->AddEntity(textInput);
	mainScene->AddEntity(createButton);
	mainScene->AddEntity(joinButton);

	Scene* serverListScene = new Scene("Server List");
	serverListScene->AddEntity(serverList);

	SceneManager::AddScene(mainScene);
	SceneManager::AddScene(gameScene);
	SceneManager::AddScene(serverListScene);

	sf::Event event;
	sf::Clock clock;
	while (m_Running)
	{
		m_Window.setView(m_Camera);
		SceneManager::GetActiveScene()->OnUpdate(clock.restart().asSeconds());

		m_Window.clear();
		SceneManager::GetActiveScene()->OnDraw(m_Window);
		m_Window.display();

		while (m_Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_Window.close();
				m_Running = false;
			}

			SceneManager::GetActiveScene()->OnEvent(event);
		}
	}
}

Application* Application::Get()
{
	if (s_Instance == nullptr)
		s_Instance = new Application();
	return s_Instance;
}

void Application::Destroy()
{
	delete s_Instance;
}
