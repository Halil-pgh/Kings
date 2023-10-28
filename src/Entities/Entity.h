#pragma once

class Entity {
public:
	Entity() = default;
	virtual ~Entity() = default;

	virtual void OnAttach() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnDraw(sf::RenderWindow& window) = 0;
	virtual bool OnEvent(const sf::Event& event) = 0;
};