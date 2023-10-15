#include "pch.h"
#include "Random.h"

void Random::Init() {
    m_RandomEngine.seed(std::random_device()());
}

template<typename T>
T Random::GenerateInt(T min, T max) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(m_RandomEngine);
}

template<typename T>
T Random::GenerateReal(T min, T max) {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(m_RandomEngine);
}