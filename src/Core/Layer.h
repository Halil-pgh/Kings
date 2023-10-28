#pragma once

#include "Entities/Entity.h"

class Layer {
public:
	Layer(std::string name);
	~Layer();

	inline const std::string& GetName() const { return m_Name; }
	inline sf::View& GetView() { return m_View; }

	// Be careful it deletes the entity from disk!
	void RemoveEntity(Entity* entity);
	void AddEntity(Entity* entity);

	void OnUpdate(float deltaTime);
	void OnDraw(sf::RenderWindow& window);
	bool OnEvent(const sf::Event& event);

private:
	sf::View m_View;
	std::vector<Entity*> m_Entities;
	std::string m_Name;
};
