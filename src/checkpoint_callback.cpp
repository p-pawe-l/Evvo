/**
 * @file checkpoint_callback.cpp
 * @brief Implements CheckpointCallback.
 */

#include "evo_callbacks.hpp"

#include <fstream>
#include <utility>

CheckpointCallback::CheckpointCallback(std::string filename, int interval)
    : filename_{std::move(filename)}, interval_{interval} {}

void CheckpointCallback::call(const GenerationStats<double>& stats) {
    if (stats.generation % this->interval_ != 0) {
        return;
    }

    std::ofstream out(this->filename_, std::ios::trunc);

    Genome<double>* best = stats.best_genome;
    out << stats.generation << ',' << stats.best_fitness;
    for (std::size_t i = 0; i < best->size(); ++i) {
        out << ',' << best->data()[i];
    }
    out << '\n';
}
