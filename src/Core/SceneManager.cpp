#include "pch.h"
#include "SceneManager.h"

std::vector<Scene*> SceneManager::m_Scenes;
Scene* SceneManager::m_ActiveScene = nullptr;

void SceneManager::Init() {
	// Nothing for now
}

void SceneManager::Destroy() {
	for (Scene* scene : m_Scenes)
		delete scene;
}

void SceneManager::AddScene(const std::string& name) {
	auto scene = new Scene(name);

	if (m_ActiveScene == nullptr)
		m_ActiveScene = scene;

	m_Scenes.push_back(scene);
}

void SceneManager::AddScene(Scene* scene) {
	m_Scenes.push_back(scene);

	if (m_ActiveScene == nullptr)
		m_ActiveScene = scene;
}

Scene* SceneManager::GetScene(const std::string& name) {
	for (Scene* scene : m_Scenes) {
		if (scene->GetName() == name)
			return scene;
	}

	std::cout << "Failed to get scene!\n";
	std::cout << "Could not found scene called '" << name << "'!\n";
	return nullptr;
}

void SceneManager::RemoveScene(const std::string& name) {
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&](Scene* scene) {
		return scene->GetName() == name;
	});

	if (it == m_Scenes.end()) {
		std::cout << "Failed to remove scene!\n";
		std::cout << "Could not found scene called '" << name << "'!\n";
		return;
	}

	delete* it;

	m_Scenes.erase(it);
}

Scene* SceneManager::GetActiveScene() {
	if (m_ActiveScene == nullptr)
		std::cout << "Warning: You are trying to get a null active scene!\n";
	return m_ActiveScene;
}

void SceneManager::SetActiveScene(const std::string& name) {
	for (Scene* scene : m_Scenes) {
		if (scene->GetName() == name) {
			m_ActiveScene = scene;
			return;
		}
	}

	std::cout << "Failed to set active scene!\n";
	std::cout << "Could not found a scene called '" << name << "'!\n";
}
