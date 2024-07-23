#pragma once

#include "Building.h"

class Mine : public Building {
public:
	Mine(float x, float y);
	Mine();

	void OnUpdate(float deltaTime) override;
	bool OnEvent(const sf::Event &event) override;

	std::string TextureName() final;
	unsigned int MoneyPerSecond();

	// Serialization
	template<class Archive>
	void serialize(Archive& archive) {
		archive(cereal::base_class<Building>(this), m_Rect);
	}
};

// Register the type and the polymorphic relation for serialization
CEREAL_REGISTER_TYPE(Mine)
// CEREAL_REGISTER_POLYMORPHIC_RELATION(Building, Mine)
