#pragma once

#include <vector>
#include <random>
#include "Data/PlayerData.h"

class Networker {
public:
	Networker() = default;
	virtual ~Networker() = default;
	virtual void Run() = 0;
	virtual void ShoutDown() = 0;

	virtual const std::vector<PlayerData>& GetPlayers() const = 0;
	virtual void SetPlayerData(const PlayerData& data) = 0;
	virtual uint64_t GetUUID() = 0;

	virtual void OnDisconnect(std::function<void(uint64_t)>) = 0;
	virtual void OnConnect(std::function<void(const PlayerData&)>) = 0;

protected:
	static uint64_t GenerateUUID() {
		std::random_device rd;
		std::mt19937_64 generator(rd());

		std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);
		return distribution(generator);
	}
};