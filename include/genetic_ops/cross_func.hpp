#pragma once

#include <algorithm>
#include <set>
#include <type_traits>

#include "../core/genome.hpp"
#include "../util/rand_util.hpp"

namespace {

// Draws k distinct indices from [0, len) by re-rolling on a collision. A set
// keeps them both unique and sorted for crossover_k_point to walk through.
std::set<int> pick_k_sorted_crossover_points(const std::size_t k, const int len) {
    std::size_t point_count = std::min(k, static_cast<std::size_t>(len));
    std::set<int> points;
    while (points.size() < point_count) {
        points.insert(random_value<int>(0, len - 1));
    }
    return points;
}

} // namespace

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

// Picks k distinct crossover points and alternates which parent's genes are
// copied for each segment between consecutive points.
template <typename T, std::size_t k>
Genome<T> crossover_k_point(const Genome<T>& parent1, const Genome<T>& parent2) {
    Genome<T> offspring = parent1;
    int len = std::min(offspring.size(), parent2.size());
    if (len == 0 || k == 0) {
        return offspring;
    }

    std::set<int> points = pick_k_sorted_crossover_points(k, len);

    bool use_parent2 = false;
    auto next_point = points.begin();
    for (int i = 0; i < len; ++i) {
        if (next_point != points.end() && i == *next_point) {
            use_parent2 = !use_parent2;
            ++next_point;
        }
        if (use_parent2) {
            offspring[i] = parent2[i];
        }
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
        offspring[i] = static_cast<T>(random_double(lo - (alpha * range), hi + (alpha * range)));
    }
    return offspring;
}

// Draws each gene from a Gaussian bell centered on the parents' midpoint;
// std_dev controls how far offspring can spread from that midpoint.
template <typename T, double std_dev>
    requires std::is_arithmetic_v<T>
Genome<T> crossover_gaussian(const Genome<T>& parent1, const Genome<T>& parent2) {
    Genome<T> offspring = parent1;
    int len = std::min(offspring.size(), parent2.size());
    for (int i = 0; i < len; ++i) {
        double mean = (static_cast<double>(offspring[i]) + static_cast<double>(parent2[i])) / 2.0;
        offspring[i] = static_cast<T>(random_gaussian(mean, std_dev));
    }
    return offspring;
}