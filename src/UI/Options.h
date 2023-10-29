#pragma once

#include "Entities/Entity.h"

class Options : public Entity {
public:
	Options();

	inline void OnDetach(const std::function<void()>& func) { m_OnDetach = func; }
	inline void OnDisconnect(const std::function<void()>& func) { m_OnDisconnect = func; }

	void OnAttach() override;
	bool OnEvent(const sf::Event &event) override;
	void OnDraw(sf::RenderWindow &window) override;
	void OnUpdate(float deltaTime) override;

private:
	std::function<void()> m_OnDetach;
	std::function<void()> m_OnDisconnect;

	sf::RectangleShape m_Rect;
	sf::Text m_Heading;
	sf::Text m_Text;
};
