#pragma once

#include <random>

class Random {
public:
    static void Init();

    template<typename T>
    static T GenerateInt(T min, T max) {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(m_RandomEngine);
    }

    template<typename T>
    static T GenerateReal(T min, T max) {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(m_RandomEngine);
    }

private:
    Random() = default;

private:
    static std::mt19937_64 m_RandomEngine;
};
