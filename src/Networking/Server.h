#pragma once

#include "Data/PlayerData.h"
#include "Client.h"

struct ClientData {
	sf::IpAddress ip;
	unsigned short port;
};

class Server : public Networker {
public:
	explicit Server(std::string name);
	~Server() override;

	void Run() override;
	void ShoutDown() final;

	inline const std::vector<PlayerData>& GetPlayers() const override { return m_Players; }
	inline void SetPlayerData(const PlayerData& data) override;
	inline uint64_t GetUUID() override { return m_Data.uuid; }

	void OnDisconnect(std::function<void(uint64_t)> function) override { m_OnDisconnect = function; }
	void OnConnect(std::function<void(const PlayerData&)> function) override { m_OnConnect = function; }

public:
	static const unsigned int PORT = 5000;

private:
	std::function<void(uint64_t)> m_OnDisconnect;
	std::function<void(PlayerData)> m_OnConnect;

	std::vector<ClientData> m_Clients;
	std::vector<PlayerData> m_Players;
	PlayerData m_Data;

	std::string m_Name;
	std::thread m_Thread;
	std::mutex m_PlayersMutex;
	bool m_Running = false;

	sf::UdpSocket m_Socket;
	sf::IpAddress m_Ip;
};
