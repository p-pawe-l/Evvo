#pragma once

#include <vector>

#include "core/genome.hpp"

// Snapshot of a single generation's evolution results, reported to every
// Callback after evaluation.
template <typename T> struct GenerationStats {
    int generation;
    const PopulationVec<T>& population;
    double best_fitness;
    double avg_fitness;
    Genome<T>* best_genome;
    const std::vector<double>& fitnesses;
};
