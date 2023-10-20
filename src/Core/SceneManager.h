#pragma once

#include "Scene.h"

class SceneManager {
public:
	static void Init();
	static void Destroy();

	static void AddScene(const std::string& name);
	static void AddScene(Scene* scene);
	static Scene* GetScene(const std::string& name);

	// Dangerous! It removes from memory also
	static void RemoveScene(const std::string& name);

	static Scene* GetActiveScene();
	static void SetActiveScene(const std::string& name);

private:
	static std::vector<Scene*> m_Scenes;
	static Scene* m_ActiveScene;
};