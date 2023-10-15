#pragma once

#include <random>

class Random {
public:
    static void Init();

    template<typename T>
    static T GenerateInt(T min, T max);

    template<typename T>
    static T GenerateReal(T min, T max);

private:
    Random() = default;

private:
    static std::mt19937_64 m_RandomEngine;
};
