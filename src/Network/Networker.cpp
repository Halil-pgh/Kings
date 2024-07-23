#include "pch.h"
#include "Networker.h"

#define ENET_IMPLEMENTATION
#include <enet/enet.h>

void Networker::initialize() {
	if (enet_initialize() != 0) {
		std::cout << "An error occurred while initializing ENet.\n";
		exit(EXIT_FAILURE);
	}
}

void Networker::deinitialize() {
	enet_deinitialize();
}

std::string Networker::hostToIp(const in6_addr& host) {
	struct sockaddr_in6 addr{};
	addr.sin6_family = AF_INET6;
	addr.sin6_addr = host;
	char buffer[INET6_ADDRSTRLEN];
	const char* result = inet_ntop(AF_INET6, &(addr.sin6_addr), buffer, INET6_ADDRSTRLEN);
	if (result == nullptr) {
		return ""; // Handle error
	}
	std::string str(result);
	std::string substring = str.substr(str.find("::ffff:") + 7);
	return substring;
}