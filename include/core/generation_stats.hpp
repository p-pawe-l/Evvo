/**
 * @file generation_stats.hpp
 * @brief Defines GenerationStats<T>, a snapshot of a single generation
 *        passed to Evolver<T>'s callback and produced by EvoPolicy<T>.
 */

#pragma once

#include <vector>

#include "core/genome.hpp"

/**
 * @brief Snapshot of a single generation's evolution results.
 * @tparam T Gene type of the genomes being evolved.
 */
template <typename T> struct GenerationStats {
    int generation;
    const PopulationVec<T>& population;
    double best_fitness;
    double avg_fitness;
    Genome<T>* best_genome;
    const std::vector<double>& fitnesses;
};
