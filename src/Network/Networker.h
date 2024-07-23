#pragma once

#include <string>

struct in6_addr;

class Networker {
public:
	static void initialize();
	static void deinitialize();
	static std::string hostToIp(const in6_addr& host);
};
