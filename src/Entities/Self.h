#pragma once

#include "Player.h"
#include "UI/PlayerStats.h"
#include "UI/ShopBar.h"
#include "Network/Client.h"
#include "Network/Server.h"

enum class Mode {
	Walk,
	Build,
	Settings
};

class Self : public Player {
public:
	Self();
	~Self() override;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow& window) override;
	bool OnEvent(const sf::Event& event) override;

	void BecomeServer(const std::string& serverName);
	void BecomeClient();
	void SetId(uint16_t id) { m_Id = id; }

	std::shared_ptr<Client> GetClient();
	std::shared_ptr<Server> GetServer();

	void Reset();
private:
	void FollowMouse();
	bool CheckBuildingsForProduction();

private:
	Mode m_Mode = Mode::Walk;
	std::unique_ptr<Building> m_ProductionBuilding;

	std::shared_ptr<ShopBar> m_ShopBar;
	std::shared_ptr<PlayerStats> m_PlayerStats;
	unsigned int m_MaxSoldier = 0;
	unsigned int m_MoneyPerSecond = 0;
	unsigned int m_Money = 0;
	float m_MoneyTime = 0;
	float m_Speed;

private:
	uint16_t m_Id;
	std::shared_ptr<Client> m_Client;
	std::shared_ptr<Server> m_Server;

	std::thread m_ServerThread;
	std::unordered_map<uint16_t, std::shared_ptr<Player>> m_Players;
};