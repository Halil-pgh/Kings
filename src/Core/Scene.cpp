#include "pch.h"
#include "Scene.h"

#include <utility>

Scene::Scene(std::string name)
	: m_Name(std::move(name)) {
}

Scene::~Scene() {
	for (Entity* entity : m_Entites)
		delete entity;
}

void Scene::AddEntity(Entity* entity) {
	m_Entites.push_back(entity);
}

void Scene::RemoveEntity(Entity* entity) {
	auto it = std::find(m_Entites.begin(), m_Entites.end(), entity);
	if (it == m_Entites.end()) {
		std::cout << "Entity couldn't found: " << entity << "\n";
		return;
	}
	m_Entites.erase(it);
	delete entity;
}

void Scene::OnUpdate(float deltaTime) {
	for (uint32_t i = 0; i < m_Entites.size(); i++)
		m_Entites[i]->OnUpdate(deltaTime);
}

void Scene::OnDraw(sf::RenderWindow& window) {
	for (uint32_t i = 0; i < m_Entites.size(); i++)
		m_Entites[i]->OnDraw(window);
}

void Scene::OnEvent(const sf::Event& event) {
	for (uint32_t i = 0; i < m_Entites.size(); i++)
		m_Entites[i]->OnEvent(event);
}
