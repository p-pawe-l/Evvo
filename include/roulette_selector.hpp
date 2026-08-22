/**
 * @file roulette_selector.hpp
 * @brief Declares RouletteSelector, a fitness-proportionate (roulette
 *        wheel) Selector implementation. Implemented in
 *        src/roulette_selector.cpp.
 */

#pragma once

#include <vector>

#include "evo_policy.hpp"
#include "selector.hpp"

/**
 * @brief Fitness-proportionate (roulette wheel) selector: builds a
 *        cumulative fitness array once at construction, then picks an
 *        individual with probability proportional to its share of total
 *        fitness in O(log n) per pick.
 */
class RouletteSelector : public Selector {
private:
    std::vector<double> cumulative_;

public:
    /**
     * @brief Builds the cumulative fitness array to pick from.
     * @param eval Fitnesses of the population to select from; must be
     *             non-empty.
     */
    explicit RouletteSelector(const PopulationEval& eval);

    [[nodiscard]] std::size_t pick() const override;
};
