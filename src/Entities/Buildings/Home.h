#pragma once

#include "Building.h"

class Home : public Building {
public:
	Home(float x, float y);

	void OnUpdate(float deltaTime) override;
	bool OnEvent(const sf::Event &event) override;

	std::string TextureName() final;

	unsigned int GetMaxCount();
};
