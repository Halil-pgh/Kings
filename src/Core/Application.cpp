#include "pch.h"
#include "Application.h"

#include "Entities/Self.h"
#include "UI/FontManager.h"
#include "UI/Button.h"
#include "UI/ServerList.h"
#include "UI/TextInput.h"
#include "Network/Networker.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Random.h"

Application* Application::s_Instance = nullptr;

Application::Application() {
	m_Window.create(sf::VideoMode(1024, 640), "Game");
	Random::Init();
	SceneManager::Init();
	Networker::initialize();
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
	Networker::deinitialize();
}

void Application::Run() {
	m_ServerList = std::make_shared<ServerList>();
	auto textInput = std::make_shared<TextInput>((float)GetWindowBase().getSize().x / 3, 3.0f * (float)GetWindowBase().getSize().y / 12, (float)GetWindowBase().getSize().x / 3, 50, "Enter your name");

	auto createButton = std::make_shared<Button>(2.0f * (float)GetWindowBase().getSize().x / 12, 7.0f * (float)GetWindowBase().getSize().y / 12, 2.0f * (float)GetWindowBase().getSize().x / 12, 50, "Create Game");
	SetButtonTheme(createButton);
	createButton->SetOnClickCallback([&]() {
		if (textInput->Get().empty())
			return;

		if (SceneManager::GetScene("Game") == nullptr)
			CreateGameScene();
		m_Self->SetName(textInput->Get());
		m_Self->BecomeServer(textInput->Get());

		SceneManager::SetActiveScene("Game");
	});

	auto joinButton = std::make_shared<Button>(8.0f * (float)GetWindowBase().getSize().x / 12, 7.0f * (float)GetWindowBase().getSize().y / 12, 2.0f * (float)GetWindowBase().getSize().x / 12, 50, "Join Game");
	SetButtonTheme(joinButton);
	joinButton->SetOnClickCallback([&]() {
		if (textInput->Get().empty())
			return;

		if (SceneManager::GetScene("Game") == nullptr)
			CreateGameScene();
		m_Self->SetName(textInput->Get());
		m_Self->BecomeClient();
		SceneManager::SetActiveScene("Game");

		if (SceneManager::GetScene("Server List") == nullptr)
			CreateServerList();
		m_ServerList->SetClient(m_Self->GetClient());
		m_ServerList->SetBackScene("Main");
		SceneManager::SetActiveScene("Server List");
	});

	auto exitButton = std::make_shared<Button>(5.0f * (float)GetWindowBase().getSize().x / 12, 8.0f * (float)GetWindowBase().getSize().y / 12, 2.0f * (float)GetWindowBase().getSize().x / 12, 50, "Exit");
	SetButtonTheme(exitButton);
	exitButton->SetOnClickCallback([&]() {
		m_Window.close();
		m_Running = false;
	});

	auto mainScene = new Scene("Main");
	SceneManager::AddScene(mainScene);
	mainScene->AddLayer("UI");
	mainScene->GetLayer("UI")->AddEntity(textInput);
	mainScene->GetLayer("UI")->AddEntity(createButton);
	mainScene->GetLayer("UI")->AddEntity(joinButton);
	mainScene->GetLayer("UI")->AddEntity(exitButton);

	sf::Event event{};
	sf::Clock clock;
	SceneManager::SetActiveScene("Main");
	while (m_Running) {
		SceneManager::GetActiveScene()->OnUpdate(clock.restart().asSeconds());

		m_Window.clear(sf::Color(231, 240, 220));
		SceneManager::GetActiveScene()->OnDraw(m_Window);
		m_Window.display();

		while (m_Window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_Window.close();
				m_Running = false;
				break;
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

sf::Vector2f Application::GetMousePosition(const sf::View& base) {
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(Application::Get()->GetWindowBase()));
	mousePos.x += base.getCenter().x - base.getSize().x / 2;
	mousePos.y += base.getCenter().y - base.getSize().y / 2;
	return mousePos;
}

void Application::DestroyGameScene() {
	SceneManager::RemoveScene("Game");
}

void Application::CreateGameScene() {
	m_Self = std::make_shared<Self>();
	auto gameScene = new Scene("Game");
	SceneManager::AddScene(gameScene);
	gameScene->AddLayer("UI");
	gameScene->AddLayer("Game");
	gameScene->GetLayer("Game")->AddEntity(m_Self);
}

void Application::DestroyServerList() {
	SceneManager::RemoveScene("Server List");
}

void Application::CreateServerList() {
	m_ServerList = std::make_shared<ServerList>();
	auto serverListScene = new Scene("Server List");
	SceneManager::AddScene(serverListScene);
	serverListScene->AddLayer("UI");
	serverListScene->GetLayer("UI")->AddEntity(m_ServerList);
}

void Application::SetButtonTheme(const std::shared_ptr<Button> &button) {
	button->SetNormalColor(sf::Color(89, 116, 69));
	button->SetHighlightedColor(sf::Color(101, 129, 71));
	button->SetPressedColor(sf::Color(114, 151, 98));
}
