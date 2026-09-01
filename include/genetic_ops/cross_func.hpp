#pragma once

#include <algorithm>

#include "core/genome.hpp"
#include "util/rand_util.hpp"

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

template <typename T>
Genome<T> crossover_uniform(const Genome<T>& parent1, const Genome<T>& parent2) {
    Genome<T> offspring = parent1;
    int len = std::min(offspring.size(), parent2.size());
    for (int i = 0; i < len; ++i) {
        if (random_double(0.0, 1.0) < 0.5) {
            offspring[i] = parent2[i];
        }
    }
    return offspring;
}

// Blends toward the midpoint of both parents; T must support addition and
// division by a double.
template <typename T>
Genome<T> crossover_arithmetic(const Genome<T>& parent1, const Genome<T>& parent2) {
    Genome<T> offspring = parent1;
    int len = std::min(offspring.size(), parent2.size());
    for (int i = 0; i < len; ++i) {
        offspring[i] = static_cast<T>((offspring[i] + parent2[i]) / 2.0);
    }
    return offspring;
}
