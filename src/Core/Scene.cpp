#include "pch.h"
#include "Scene.h"

#include <utility>
#include <cassert>

Scene::Scene(std::string name)
	: m_Name(std::move(name)) {
}

Scene::~Scene() {
	for (uint32_t i = 1; i <= m_Layers.size(); i++)
		delete m_Layers[i];
}

void Scene::AddLayer(const std::string& layerName) {
	auto layer = new Layer(layerName);
	m_Layers[m_Layers.size() + 1] = layer;
}

void Scene::AddLayer(const std::string& layerName, uint32_t priority) {
	CreateSpace(priority);
	auto layer = new Layer(layerName);
	m_Layers[priority] = layer;
}

void Scene::RemoveLayer(const std::string& layerName) {
	for (uint32_t i = 1; i <= m_Layers.size(); i++) {
		if (m_Layers[i]->GetName() == layerName) {
			RemoveSpace(i);
			return;
		}
	}
	std::cout << "Failed to remove/find " << layerName << " layer!\n";
}

void Scene::ChangePriority(const std::string& layerName, uint32_t priority) {
	auto oldPriority = GetPriority(layerName);
	if (oldPriority == priority)
		return;

	CreateSpace(priority);
	auto layer = new Layer(layerName);
	m_Layers[priority] = layer;

	if (oldPriority > priority)
		RemoveSpace(oldPriority + 1);
	else
		RemoveSpace(oldPriority);
}

uint32_t Scene::GetPriority(const std::string &layerName) {
	for (uint32_t i = 1; i <= m_Layers.size(); i++) {
		if (m_Layers[i]->GetName() == layerName) {
			return i;
		}
	}
	std::cout << "Failed to find " << layerName << " layer!\n";
	assert(false);
	return 0;
}

Layer* Scene::GetLayer(const std::string& name) {
	for (uint32_t i = 1; i <= m_Layers.size(); i++) {
		if (m_Layers[i]->GetName() == name) {
			return m_Layers[i];
		}
	}
	std::cout << "Failed to find " << name << " layer!\n";
	assert(false);
	return nullptr;
}

Layer* Scene::GetLayer(uint32_t priority) {
	return m_Layers[priority];
}

Layer* Scene::GetMostPriorLayer() {
	return m_Layers[1];
}

Layer* Scene::GetLeastPriorLayer() {
	return m_Layers[m_Layers.size()];
}

void Scene::OnUpdate(float deltaTime) {
	for (uint32_t i = 1; i <= m_Layers.size(); i++)
		m_Layers[i]->OnUpdate(deltaTime);
}

void Scene::OnDraw(sf::RenderWindow& window) {
	for (uint32_t i = m_Layers.size(); i > 0; i--)
		m_Layers[i]->OnDraw(window);
}

bool Scene::OnEvent(const sf::Event& event) {
	for (uint32_t i = 1; i <= m_Layers.size(); i++) {
		if (m_Layers[i]->OnEvent(event))
			return true;
	}
	return false;
}

void Scene::CreateSpace(uint32_t place) {
	if (place > m_Layers.size()) {
		std::cout << "Failed to create space that is bigger than layers size!\n";
		return;
	}
	for (uint32_t i = m_Layers.size(); i >= place; i--) {
		m_Layers[i + 1] = m_Layers[i];
	}
}

void Scene::RemoveSpace(uint32_t place) {
	delete m_Layers[place];
	for (uint32_t i = place; i < m_Layers.size(); i++) {
		m_Layers[i] = m_Layers[i + 1];
	}
	m_Layers.erase(m_Layers.size());
}
