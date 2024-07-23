#pragma once

#include "Entities/Entity.h"
#include "TextInput.h"
#include "Button.h"

class ServerAdder : public Entity {
public:
	ServerAdder();

	inline void OnServerAdd(const std::function<void(const std::string&, const std::string&)>& callback) { m_OnServerAdd = callback; }

	void OnAttach() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow &window) override;
	bool OnEvent(const sf::Event &event) override;

private:
	sf::RectangleShape m_Rect;
	std::shared_ptr<TextInput> m_NameInput;
	std::shared_ptr<TextInput> m_IPInput;
	std::shared_ptr<Button> m_AddButton;
	std::shared_ptr<Button> m_BackButton;

	std::function<void(const std::string& name, const std::string& ip)> m_OnServerAdd;
};
