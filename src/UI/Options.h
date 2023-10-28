#pragma once

#include "Entities/Entity.h"

class Options : public Entity {
public:
	Options();

	void OnAttach() override;
	bool OnEvent(const sf::Event &event) override;
	void OnDraw(sf::RenderWindow &window) override;
	void OnUpdate(float deltaTime) override;

private:
	sf::RectangleShape m_Rect;
	sf::Text m_Heading;
	sf::Text m_Text;
};
