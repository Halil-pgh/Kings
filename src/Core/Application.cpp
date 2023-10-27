#include "pch.h"
#include "Application.h"

#include "Entities/Player.h"
#include "Entities/Self.h"
#include "UI/FontManager.h"
#include "UI/Button.h"
#include "UI/ServerList.h"
#include "UI/TextInput.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Random.h"

Application* Application::s_Instance = nullptr;

Application::Application() {
	m_Window.create(sf::VideoMode(1024, 640), "Game");
	m_Camera = m_Window.getDefaultView();
	Random::Init();
	SceneManager::Init();
	FontManager::SetFont("normal", "assets/fonts/noto-sans/NotoSans-Regular.ttf");
	FontManager::SetFont("emoji", "assets/fonts/noto-emoji/NotoColorEmoji-Regular.ttf");
	TextureManager::AddTexture("home", "assets/images/home.png");
	TextureManager::AddTexture("mine", "assets/images/mine.png");

#ifdef WIN32
	// Just to see console output on debug mode (clion)
	setvbuf(stdout, nullptr, _IONBF, 0);
#endif
}

Application::~Application() {
	SceneManager::Destroy();
	FontManager::Destroy();
	TextureManager::Destroy();
}

void Application::Run() {
	auto self = new Self();
	auto textInput = new TextInput((float)GetWindowBase().getSize().x / 3, 150, (float)GetWindowBase().getSize().x / 3, 50, "Enter your name");
	auto serverList = new ServerList();

	auto createButton = new Button((float)GetWindowBase().getSize().x / 6, 400, 200, 50, "Create Game");
	createButton->SetOnClickCallback([&]() {
		if (textInput->Get().empty())
			return;
		self->SetName(textInput->Get());
		self->BecomeServer(textInput->Get());

		SceneManager::SetActiveScene("Game");
	});

	auto joinButton = new Button((float)7 * (float)GetWindowBase().getSize().x / 12, 400, 200, 50, "Join Game");
	joinButton->SetOnClickCallback([&]() {
		if (textInput->Get().empty())
			return;
		self->SetName(textInput->Get());
		self->BecomeClient();
		serverList->SetClient(self->GetClient());
		serverList->SetBackScene("Main");

		SceneManager::SetActiveScene("Server List");
	});

	auto gameScene = new Scene("Game");
	gameScene->AddEntity(self);

	auto mainScene = new Scene("Main");
	mainScene->AddEntity(textInput);
	mainScene->AddEntity(createButton);
	mainScene->AddEntity(joinButton);

	auto serverListScene = new Scene("Server List");
	serverListScene->AddEntity(serverList);

	SceneManager::AddScene(mainScene);
	SceneManager::AddScene(gameScene);
	SceneManager::AddScene(serverListScene);

	sf::Event event{};
	sf::Clock clock;
	while (m_Running) {
		m_Window.setView(m_Camera);
		SceneManager::GetActiveScene()->OnUpdate(clock.restart().asSeconds());

		m_Window.clear();
		SceneManager::GetActiveScene()->OnDraw(m_Window);
		m_Window.display();

		while (m_Window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_Window.close();
				m_Running = false;
			}

			SceneManager::GetActiveScene()->OnEvent(event);
		}
	}
}

Application* Application::Get() {
	if (s_Instance == nullptr)
		s_Instance = new Application();
	return s_Instance;
}

void Application::Destroy() {
	delete s_Instance;
}

sf::Vector2f Application::GetMousePosition() {
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(Application::Get()->GetWindowBase()));
	mousePos.x += Application::Get()->GetCamera().getCenter().x -
			Application::Get()->GetCamera().getSize().x / 2;
	mousePos.y += Application::Get()->GetCamera().getCenter().y -
			Application::Get()->GetCamera().getSize().y / 2;
	return mousePos;
}
