/**
 * @file roulette_selector.hpp
 * @brief Declares RouletteSelector, a fitness-proportionate (roulette
 *        wheel) Selector implementation. Implemented in
 *        src/roulette_selector.cpp.
 */

#pragma once

#include "../core/evo_policy.hpp"
#include "selector.hpp"

/**
 * @brief Fitness-proportionate (roulette wheel) selector: builds a
 *        cumulative fitness array once at construction, then picks an
 *        individual with probability proportional to its share of total
 *        fitness in O(log n) per pick.
 */
class RouletteSelector : public Selector<PopulationEval> {
private:
    double total_fitness_;
    const std::vector<double> *fitnesses_;    

public:
    void build_from_eval(const PopulationEval& eval) override;

    [[nodiscard]] std::size_t pick() const override;
};
