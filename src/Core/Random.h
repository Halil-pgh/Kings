#pragma once

#include <random>
#include <mutex>

class Random {
public:
	static void Init();

	template<typename T>
	static T GenerateInt(T min, T max) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		std::uniform_int_distribution<T> dist(min, max);
		return dist(m_RandomEngine);
	}

	template<typename T>
	static T GenerateReal(T min, T max) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		std::uniform_real_distribution<T> dist(min, max);
		return dist(m_RandomEngine);
	}

private:
	Random() = default;

private:
	static std::mt19937_64 m_RandomEngine;
	static std::mutex m_Mutex; // Players are also created on the server thread
};
