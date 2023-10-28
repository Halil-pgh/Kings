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
	SceneManager::AddScene(gameScene);
	gameScene->AddLayer("UI");
	gameScene->AddLayer("Game");
	gameScene->GetLayer("Game")->AddEntity(self);

	auto mainScene = new Scene("Main");
	SceneManager::AddScene(mainScene);
	mainScene->AddLayer("UI");
	mainScene->GetLayer("UI")->AddEntity(textInput);
	mainScene->GetLayer("UI")->AddEntity(createButton);
	mainScene->GetLayer("UI")->AddEntity(joinButton);

	auto serverListScene = new Scene("Server List");
	SceneManager::AddScene(serverListScene);
	serverListScene->AddLayer("UI");
	serverListScene->GetLayer("UI")->AddEntity(serverList);

	sf::Event event{};
	sf::Clock clock;
	SceneManager::SetActiveScene("Main");
	while (m_Running) {
		SceneManager::GetActiveScene()->OnUpdate(clock.restart().asSeconds());

		m_Window.clear();
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
