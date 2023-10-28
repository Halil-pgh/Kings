#pragma once

#include "Layer.h"

// How this is going to work ???
// Changing the priority of a layer
/*
 * 1. UI
 * 2. Game
 *
 * -- we want to add pop-up menu
 * -- there is two approach
 *
 * - Long way
 * AddLayer("Pop-Up");
 * 1. UI
 * 2. Game
 * 3. Pop-Up
 * ChangePriority("Pop-Up", 1);
 * 1. Pop-Up
 * 2. UI
 * 3. Game
 *
 * - Short way
 * AddLayer("Pop-Up", 1);
 * 1. Pop-Up
 * 2. UI
 * 3. Game
 *
 */

class Scene {
public:
	explicit Scene(std::string name);
	~Scene();

	inline const std::string& GetName() const { return m_Name; }

	void AddLayer(const std::string& layerName);
	void AddLayer(const std::string& layerName, uint32_t priority);
	void RemoveLayer(const std::string& layerName);

	void ChangePriority(const std::string& layerName, uint32_t priority);
	uint32_t GetPriority(const std::string& layerName);
	Layer* GetLayer(const std::string& name);
	Layer* GetLayer(uint32_t priority);
	Layer* GetMostPriorLayer();
	Layer* GetLeastPriorLayer();

	void OnUpdate(float deltaTime);
	void OnDraw(sf::RenderWindow& window);
	bool OnEvent(const sf::Event& event);

private:
	void CreateSpace(uint32_t place);
	void RemoveSpace(uint32_t place);

private:
	std::map<uint32_t, Layer*> m_Layers;
	std::string m_Name;
};