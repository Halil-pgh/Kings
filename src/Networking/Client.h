#pragma once

#include "Networker.h"

struct ServerInfo
{
	sf::IpAddress ip;
	unsigned short port;
	std::string name;
	unsigned int playerCount;

	bool operator==(const ServerInfo& other) const
	{
		return other.ip == ip && other.port == port;
	}
};

class Client : public Networker
{
public:
	Client();
	~Client() override;

	void Run() override;
	void ShoutDown() override { m_Thread.join(); }

	inline const std::vector<PlayerData>& GetPlayers() const override { return m_Players; }
	inline void SetPlayerData(const PlayerData& data) override { m_Data = data; }
	inline uint64_t GetUUID() override { return m_Data.uuid; }

	void RefreshServers();
	void JoinServer(const ServerInfo& server);

private:
	PlayerData m_Data;
	std::vector<PlayerData> m_Players;

	sf::UdpSocket m_Socket;
	std::thread m_Thread;

	std::vector<ServerInfo> m_ServerInfos;

	bool m_Connected = false;

	friend class ServerList;
};
