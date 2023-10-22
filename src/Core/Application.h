#pragma once

#include "Scene.h"

class Application {
public:
	static Application* Get();
	static void Destroy();
	void Run();

	static sf::Vector2f GetMousePosition();
	inline const sf::WindowBase& GetWindowBase() const { return m_Window; }
	inline bool IsRunning() const { return m_Running; }
	inline sf::View& GetCamera() { return m_Camera; }

private:
	Application();
	~Application();

private:
	static Application* s_Instance;

	sf::RenderWindow m_Window;
	sf::View m_Camera;
	bool m_Running = true;
};

