/**
 * @file rand_util.hpp
 * @brief Small random-number helper shared by mut_func.hpp and
 *        cross_func.hpp.
 */

#pragma once

#include <random>

namespace {
    /**
     * @brief Generates a uniformly distributed random value in [lo, hi].
     * @return A random double in [lo, hi].
     */
    double random_double(double lo, double hi) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(lo, hi);
        return dist(gen);
    }
}
