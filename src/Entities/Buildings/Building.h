#pragma once

#include "Entities/Entity.h"

// For serialization
#include <cereal/types/polymorphic.hpp>

enum class BuildingType {
	Home = 0,
	Mine = 1,

	Count = 2 // <- This is count, do not use that for BuildingType
};

class Building : public Entity {
public:
	Building(float x, float y, BuildingType type);
	Building() = default;
	~Building() override = default;

	void OnAttach() override {}
	void OnUpdate(float deltaTime) override = 0;
	void OnDraw(sf::RenderWindow &window) override;
	bool OnEvent(const sf::Event &event) override = 0;

	virtual std::string TextureName() = 0;

	inline sf::FloatRect GetRectangle() const { return m_Rect.getGlobalBounds(); }
	inline BuildingType GetType() const { return m_Type; }
	inline const sf::Vector2f& GetPosition() const { return m_Rect.getPosition(); }
	inline void SetPosition(const sf::Vector2f& position) { m_Rect.setPosition(position); }

	void SetProduction(bool production);
	void SetProductable(bool productable);

	// Serialization
	template<class Archive>
	void serialize(Archive& archive) {
		archive(m_Rect, m_Type, m_IsProductable);
	}

protected:
	BuildingType m_Type;
	sf::RectangleShape m_Rect;
	bool m_IsProductable = true;
	sf::Texture* m_UnproductableTexture = nullptr;
};
