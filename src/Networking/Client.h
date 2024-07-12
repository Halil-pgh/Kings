#pragma once

#include <utility>

#include "Networker.h"

struct ServerInfo {
	sf::IpAddress ip;
	unsigned short port;
	std::string name;
	unsigned int playerCount;

	bool operator==(const ServerInfo& other) const {
		return other.ip == ip && other.port == port;
	}
};

class Client : public Networker {
public:
	Client();
	~Client() override;

	void Run() override;
	void ShoutDown() final;

	inline const std::vector<PlayerData>& GetPlayers() const override { return m_Players; }
	inline void SetPlayerData(const PlayerData& data) override { m_Data = data; }
	inline uint64_t GetUUID() override { return m_Data.uuid; }
	inline bool IsInGame() const { return m_IsInGame | !m_Connected; }
	void OnDisconnect(std::function<void(uint64_t)> function) override { m_OnDisconnect = std::move(function); }
	void OnConnect(std::function<void(const PlayerData&)> function) override { m_OnConnect = std::move(function); }

	void RefreshServers();
	void JoinServer(const ServerInfo& server);

private:
	std::function<void(uint64_t)> m_OnDisconnect;
	std::function<void(PlayerData)> m_OnConnect;

private:
	PlayerData m_Data;
	std::vector<PlayerData> m_Players;
	sf::IpAddress m_ServerIp;

	unsigned short m_ServerPort{};
	sf::UdpSocket m_Socket;

	std::thread m_Thread;
	std::vector<ServerInfo> m_ServerInfos;

	bool m_IsInGame = false;
	bool m_Running = false;
	bool m_Connected = false;

	friend class ServerList;
};
