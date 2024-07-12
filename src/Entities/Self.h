#pragma once

#include "Player.h"
#include "UI/PlayerStats.h"
#include "UI/ShopBar.h"
#include "Networking/Client.h"
#include "Networking/Server.h"

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
	void OnUpdate(float deltaTime) override;
	void OnDraw(sf::RenderWindow& window) override;
	bool OnEvent(const sf::Event& event) override;

	void BecomeServer(const std::string& serverName);
	void BecomeClient();

	Client** GetClient();
	Server* GetServer();

private:
	void FollowMouse();
	void HandleConnection();
	void InitNetworker();
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

private:
	float m_Speed;
	Networker* m_Networker;

	std::vector<std::shared_ptr<Player>> m_OtherPlayers;
	std::vector<uint64_t> m_JoinedUUIDs;
};