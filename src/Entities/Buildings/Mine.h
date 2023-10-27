#pragma once

#include "Building.h"

class Mine : public Building {
public:
	Mine(float x, float y);

	void OnUpdate(float deltaTime) override;
	void OnEvent(const sf::Event &event) override;

	std::string TextureName() final;

	unsigned int MoneyPerSecond();
};
