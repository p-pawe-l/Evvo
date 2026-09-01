#pragma once

#include <random>
#include <type_traits>

namespace {

double random_double(double lo, double hi) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(lo, hi);
    return dist(gen);
}

// Picks std::uniform_int_distribution or std::uniform_real_distribution
// based on whether T is an integral type.
template <typename T> T random_value(T lo, T hi) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dist(lo, hi);
        return dist(gen);
    } else {
        std::uniform_real_distribution<T> dist(lo, hi);
        return dist(gen);
    }
}

} // namespace
