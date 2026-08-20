/**
 * @file print_callback.cpp
 * @brief Implements PrintCallback.
 */

#include "evo_callbacks.hpp"

#include <iostream>

void PrintCallback::call(const GenerationStats<double>& stats) {
    Genome<double>* best = stats.best_genome;

    std::cout << "gen " << stats.generation
               << " | best fitness " << stats.best_fitness
               << " | avg fitness " << stats.avg_fitness
               << " | genome:";
    for (std::size_t i = 0; i < best->size(); ++i) {
        std::cout << ' ' << best->data()[i];
    }
    std::cout << '\n';
}
