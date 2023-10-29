#pragma once

#include "Scene.h"
#include "Entities/Self.h"

class Application {
public:
	static Application* Get();
	static void Destroy();
	void Run();

	static sf::Vector2f GetMousePosition(const sf::View& base);
	inline const sf::WindowBase& GetWindowBase() const { return m_Window; }
	inline bool IsRunning() const { return m_Running; }
	inline const sf::View& GetDefaultView() { return m_Window.getDefaultView(); }

	void DestroyGameScene();
	void CreateGameScene();
	void DestroyServerList();
	void CreateServerList();

private:
	Application();
	~Application();

private:
	static Application* s_Instance;

	sf::RenderWindow m_Window;
	bool m_Running = true;
	Self* m_Self;
	ServerList* m_ServerList;

	friend class Options;
};

