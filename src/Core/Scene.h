#pragma once

#include "Entities/Entity.h"

class Scene
{
public:
	Scene(std::string  name);
	~Scene();

	inline const std::string& GetName() const { return m_Name; }

	void AddEntity(Entity* entity);

	// Be carefull it deletes the entity from disk!
	void RemoveEntity(Entity* entity);

	void OnUpdate(float deltaTime);
	void OnDraw(sf::RenderWindow& window);
	void OnEvent(const sf::Event& event);

private:
	std::vector<Entity*> m_Entites;
	std::string m_Name;
};