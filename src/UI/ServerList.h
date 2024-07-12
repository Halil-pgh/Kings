#pragma once

#include "UI/Button.h"
#include "Entities/Entity.h"
#include "Networking/Client.h"

class ServerList : public Entity {
public:
	ServerList();

	void SetBackScene(const std::string& name);
	void SetClient(Client** client);

	void OnAttach() override {}
	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow& window) override;
	bool OnEvent(const sf::Event& event) override;

private:
	std::shared_ptr<Button> m_BackButton;
	std::shared_ptr<Button> m_RefreshButton;
	std::shared_ptr<Button> m_JoinButton;
	std::vector<Button> m_ServerButtons;
	std::vector<ServerInfo> m_ServerInfos;
	ServerInfo m_SelectedServerInfo;

	Client** m_Client = nullptr;
};