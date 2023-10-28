#pragma once

#include "Networking/Data/BuildingData.h"
#include "Entities/Entity.h"

class Building : public Entity {
public:
	Building(float x, float y);
	~Building() override = default;

	void OnAttach() override {}
	void OnUpdate(float deltaTime) override = 0;
	void OnDraw(sf::RenderWindow &window) override;
	bool OnEvent(const sf::Event &event) override = 0;

	virtual std::string TextureName() = 0;

	inline sf::FloatRect GetRectangle() const { return m_Rect.getGlobalBounds(); }
	inline const sf::Vector2f& GetPosition() const { return m_Rect.getPosition(); }
	inline void SetPosition(const sf::Vector2f& position) { m_Rect.setPosition(position); }

	void SetProduction(bool production);
	void SetProductable(bool productable);

protected:
	sf::RectangleShape m_Rect;
	bool m_IsProductable = true;
	sf::Texture* m_UnproductableTexture = nullptr;
};
