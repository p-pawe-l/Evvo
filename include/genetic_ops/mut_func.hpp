#pragma once

#include <algorithm>
#include <random>

#include "core/genome.hpp"
#include "util/rand_util.hpp"

template <typename T> Genome<T> mutate_gaussian(const Genome<T>& individual) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<double> noise(0.0, 0.1);
    Genome<T> offspring = individual;
    std::ranges::for_each(offspring, [](T& gene) { gene += noise(gen); });
    return offspring;
}

template <typename T> Genome<T> mutate_random_reset(const Genome<T>& individual) {
    Genome<T> offspring = individual;
    if (offspring.empty()) {
        return offspring;
    }
    int i = static_cast<int>(random_double(0.0, offspring.size()));
    offspring[i] = static_cast<T>(random_double(-1.0, 1.0));
    return offspring;
}

template <typename T> Genome<T> mutate_boundary(const Genome<T>& individual) {
    Genome<T> offspring = individual;
    std::ranges::generate(
        offspring, [] { return static_cast<T>((random_double(0.0, 1.0) < 0.5) ? -1.0 : 1.0); });
    return offspring;
}
