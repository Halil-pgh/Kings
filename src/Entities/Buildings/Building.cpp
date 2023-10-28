#include "Building.h"

#include "Core/TextureManager.h"

Building::Building(float x, float y, BuildingType type)
	: m_Type(type), m_Rect({100, 100}) {
	m_Rect.setPosition(x, y);
	m_Rect.setOrigin(m_Rect.getSize().x / 2, m_Rect.getSize().y / 2);
}

void Building::OnDraw(sf::RenderWindow &window) {
	window.draw(m_Rect);
}

void Building::SetProduction(bool production) {
	sf::Color color = m_Rect.getFillColor();
	if (production)
		color.a = 128;
	else
		color.a = 255;
	m_Rect.setFillColor(color);
}

void Building::SetProductable(bool productable) {
	if (m_IsProductable == productable) {
		return;
	}

	if (productable) {
		// Cleaning the old one
		delete m_UnproductableTexture;
		m_UnproductableTexture = nullptr;

		m_Rect.setTexture(TextureManager::GetTexture(TextureName()));
	}
	else {
		auto image = m_Rect.getTexture()->copyToImage();
		for (unsigned int x = 0; x < image.getSize().x; x++) {
			for (unsigned int y = 0; y < image.getSize().y; y++) {
				sf::Color pixel = image.getPixel(x, y);
				if (pixel.r <= 155) {
					pixel.r += 100;
				}
				else {
					pixel.r = 255;
				}
				image.setPixel(x, y, pixel);
			}
		}

		delete m_UnproductableTexture;
		m_UnproductableTexture = new sf::Texture(*m_Rect.getTexture());
		m_UnproductableTexture->loadFromImage(image);
		m_Rect.setTexture(m_UnproductableTexture);
	}
	m_IsProductable = productable;
}
