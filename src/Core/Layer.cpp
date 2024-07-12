#include "pch.h"
#include "Layer.h"

#include "Application.h"

Layer::Layer(std::string name)
	: m_Name(std::move(name)) {
	m_View = Application::Get()->GetDefaultView();
}

Layer::~Layer() = default;

void Layer::AddEntity(const std::shared_ptr<Entity>& entity) {
	m_Entities.push_back(entity);
	entity->OnAttach();
}

void Layer::RemoveEntity(const std::shared_ptr<Entity>& entity) {
	auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
	if (it == m_Entities.end()) {
		std::cout << "Entity couldn't found: " << entity << "\n";
		return;
	}
	m_Entities.erase(it);
}

void Layer::OnUpdate(float deltaTime) {
	for (uint32_t i = 0; i < m_Entities.size(); i++) {
		m_Entities[i]->OnUpdate(deltaTime);
	}
}

void Layer::OnDraw(sf::RenderWindow &window) {
	window.setView(m_View);
	for (uint32_t i = 0; i < m_Entities.size(); i++) {
		m_Entities[i]->OnDraw(window);
	}
}

bool Layer::OnEvent(const sf::Event &event) {
	for (uint32_t i = 0; i < m_Entities.size(); i++) {
		if (m_Entities[i]->OnEvent(event))
			return true;
	}
	return false;
}
