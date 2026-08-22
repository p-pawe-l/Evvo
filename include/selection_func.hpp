#pragma once

#include <cstddef>

#include "evo_policy.hpp"

/**
 * @brief Fitness-proportionate (roulette wheel) selection: picks an
 *        individual with probability proportional to its fitness by
 *        drawing a random point in [0, total_fitness] and returning
 *        the index whose cumulative fitness slice contains it.
 * @param eval Fitnesses and total fitness of the population to select
 *             from; must be non-empty.
 * @return Index into eval.fitnesses of the selected individual.
 */
std::size_t roulette_selection(const PopulationEval& eval);

std::size_t linear_rank_selection(const PopulationEval& eval);

std::size_t exp_rank_selection(const PopulationEval& eval);

std::size_t steady_state_selection(const PopulationEval& eval);

std::size_t tournament_selection(const PopulationEval& eval);

std::size_t truncation_selection(const PopulationEval& eval);

std::size_t elitist_selection(const PopulationEval& eval);

std::size_t boltzmann_selection(const PopulationEval& eval);

std::size_t lexicase_selection(const PopulationEval& eval);
