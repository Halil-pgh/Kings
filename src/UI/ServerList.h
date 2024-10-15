#pragma once

#include "UI/Button.h"
#include "Entities/Entity.h"
#include "Network/Client.h"

struct ServerInfo {
	std::string name;
	std::string ip;
};

class ServerList : public Entity {
public:
	ServerList();

	void SetBackScene(const std::string& name);
	void SetClient(const std::shared_ptr<Client>& client);

	void OnAttach() override {}
	void OnDetach() override {}
	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow& window) override;
	bool OnEvent(const sf::Event& event) override;

private:
	std::shared_ptr<Button> m_BackButton;
	std::shared_ptr<Button> m_RefreshButton;
	std::shared_ptr<Button> m_JoinButton;
	std::shared_ptr<Button> m_AddServerButton;
	std::vector<std::shared_ptr<Button>> m_ServerButtons;
	std::vector<ServerInfo> m_ServerInfos;
	ServerInfo m_SelectedServerInfo;

	std::shared_ptr<Client> m_Client;
};