#pragma once

#include <algorithm>
#include <type_traits>

#include "../core/genome.hpp"
#include "../util/rand_util.hpp"

template <typename T>
Genome<T> crossover_single_point(const Genome<T>& parent1, const Genome<T>& parent2) {
    Genome<T> offspring = parent1;
    int len = std::min(offspring.size(), parent2.size());
    if (len == 0) {
        return offspring;
    }
    int point = static_cast<int>(random_double(0.0, len));
    for (int i = point; i < len; ++i) {
        offspring[i] = parent2[i];
    }
    return offspring;
}

template <typename T, double cross_chance>
Genome<T> crossover_uniform(const Genome<T>& parent1, const Genome<T>& parent2) {
    Genome<T> offspring = parent1;
    int len = std::min(offspring.size(), parent2.size());
    for (int i = 0; i < len; ++i) {
        if (random_double(0.0, 1.0) < cross_chance) {
            offspring[i] = parent2[i];
        }
    }
    return offspring;
}

// Blends toward the midpoint of both parents.
template <typename T>
    requires std::is_arithmetic_v<T>
Genome<T> crossover_arithmetic(const Genome<T>& parent1, const Genome<T>& parent2) {
    Genome<T> offspring = parent1;
    int len = std::min(offspring.size(), parent2.size());
    for (int i = 0; i < len; ++i) {
        offspring[i] = static_cast<T>((offspring[i] + parent2[i]) / 2.0);
    }
    return offspring;
}

// BLX-alpha: draws each gene uniformly from [min(p1,p2) - alpha*range,
// max(p1,p2) + alpha*range], so offspring can land outside both parents'
// values instead of shrinking toward their mean like crossover_arithmetic.
template <typename T, double alpha>
    requires std::is_arithmetic_v<T>
Genome<T> crossover_blend(const Genome<T>& parent1, const Genome<T>& parent2) {
    Genome<T> offspring = parent1;
    int len = std::min(offspring.size(), parent2.size());
    for (int i = 0; i < len; ++i) {
        auto lo = static_cast<double>(std::min(offspring[i], parent2[i]));
        auto hi = static_cast<double>(std::max(offspring[i], parent2[i]));
        double range = hi - lo;
        offspring[i] = static_cast<T>(random_double(lo - alpha * range, hi + alpha * range));
    }
    return offspring;
}

template <typename T> Genome<T> crossover(const Genome<T>& parent1, const Genome<T>& parent2);