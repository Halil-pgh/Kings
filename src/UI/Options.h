#pragma once

#include "Entities/Entity.h"

class Options : public Entity {
public:
	Options();

	inline void OnDetach(const std::function<void()>& func) { m_OnDetach = func; }
	inline void OnCloseServer(const std::function<void()>& func) { m_OnCloseServer = func; }

	void OnAttach() override;
	bool OnEvent(const sf::Event &event) override;
	void OnDraw(sf::RenderWindow &window) override;
	void OnUpdate(float deltaTime) override;

private:
	std::function<void()> m_OnDetach;
	std::function<void()> m_OnCloseServer;

	sf::RectangleShape m_Rect;
	sf::Text m_Heading;
	sf::Text m_Text;
};
