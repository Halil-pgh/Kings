#include "pch.h"
#include "Random.h"

std::mt19937_64 Random::m_RandomEngine;

void Random::Init() {
	m_RandomEngine.seed(std::random_device()());
}
