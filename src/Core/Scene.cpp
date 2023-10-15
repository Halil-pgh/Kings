#include "pch.h"
#include "Scene.h"

Scene::Scene(const std::string& name)
	: m_Name(name)
{
}

Scene::~Scene()
{
	for (Entity* entity : m_Entites)
		delete entity;
}

void Scene::AddEntity(Entity* entity)
{
	m_Entites.push_back(entity);
}

void Scene::RemoveEntity(Entity* entity)
{
	auto it = std::find(m_Entites.begin(), m_Entites.end(), entity);
	if (it == m_Entites.end())
	{
		std::cout << "Entity couldnt found: " << entity << "\n";
		return;
	}
	m_Entites.erase(it);
	delete entity;
}

void Scene::OnUpdate(float deltaTime)
{
	for (Entity* entity : m_Entites)
		entity->OnUpdate(deltaTime);
}

void Scene::OnDraw(sf::RenderWindow& window)
{
	for (Entity* entity : m_Entites)
		entity->OnDraw(window);
}

void Scene::OnEvent(const sf::Event& event)
{
	for (Entity* entity : m_Entites)
		entity->OnEvent(event);
}
