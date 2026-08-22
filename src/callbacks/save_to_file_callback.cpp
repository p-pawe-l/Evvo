/**
 * @file save_to_file_callback.cpp
 * @brief Implements SaveToFileCallback.
 */

#include "callbacks/evo_callbacks.hpp"

#include <fstream>
#include <utility>

SaveToFileCallback::SaveToFileCallback(std::string filename) : filename_{std::move(filename)} {
    // Truncate any previous contents so each run starts a fresh file.
    std::ofstream out(this->filename_, std::ios::trunc);
}

void SaveToFileCallback::call(const GenerationStats<double>& stats) {
    std::ofstream out(this->filename_, std::ios::app);

    Genome<double>* best = stats.best_genome;
    out << stats.generation << ',' << stats.best_fitness << ',' << stats.avg_fitness;
    for (std::size_t i = 0; i < best->size(); ++i) {
        out << ',' << best->data()[i];
    }
    out << '\n';
}
