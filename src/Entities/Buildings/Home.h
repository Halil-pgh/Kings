#pragma once

#include "Building.h"

class Home : public Building {
public:
    Home(float x, float y);

    void OnUpdate(float deltaTime) override;
    void OnEvent(const sf::Event &event) override;
};
