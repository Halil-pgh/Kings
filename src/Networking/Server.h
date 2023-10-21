#pragma once

#include "Data/PlayerData.h"
#include "Client.h"

class Server : public Networker {
public:
	explicit Server(std::string name);
	~Server() override;

	void Run() override;
	void ShoutDown() override;

	inline const std::vector<PlayerData>& GetPlayers() const override { return m_Players; }
	inline void SetPlayerData(const PlayerData& data) override;
	inline uint64_t GetUUID() override { return m_Data.uuid; }

public:
	static const unsigned int PORT = 5000;

private:
	std::vector<PlayerData> m_Players;
	PlayerData m_Data;

	std::string m_Name;
	std::thread m_Thread;
	std::mutex m_PlayersMutex;
	bool m_Running = false;

	sf::UdpSocket m_Socket;
	sf::IpAddress m_Ip;
};
