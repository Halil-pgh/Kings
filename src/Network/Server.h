#pragma once

#include <enet/enet.h>

#include "Entities/Player.h"

#define SERVER_ID UINT16_MAX

class Server {
public:
	explicit Server(uint16_t port);
	~Server();

	void update();
	bool someClientConnected();
	bool isRunning() const { return running; }
	void stop() { running = false; }
public:
	void send(ENetPeer *peer, const void *data, size_t size);
	void broadcast(const void *data, size_t size);
	void broadcastExcept(ENetPeer *peer, const void *data, size_t size);
	void disconnect(ENetPeer *peer);

	static const unsigned int PORT = 7777;

private:
	ENetAddress address{};
	ENetHost *host = nullptr;
	bool running = true;

	std::unordered_map<uint16_t, std::shared_ptr<Player>> players;
};
