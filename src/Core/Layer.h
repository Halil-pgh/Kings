#pragma once

#include "Entities/Entity.h"

class Layer {
public:
	explicit Layer(std::string name);
	~Layer();

	inline const std::string& GetName() const { return m_Name; }
	inline sf::View& GetView() { return m_View; }

	void RemoveEntity(const std::shared_ptr<Entity>& entity);
	void AddEntity(const std::shared_ptr<Entity>& entity);

	void OnUpdate(float deltaTime);
	void OnDraw(sf::RenderWindow& window);
	bool OnEvent(const sf::Event& event);

private:
	sf::View m_View;
	std::vector<std::shared_ptr<Entity>> m_Entities;
	std::string m_Name;
};
