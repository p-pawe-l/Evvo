#include "callbacks/evo_callbacks.hpp"

#include <fstream>
#include <utility>

CheckpointCallback::CheckpointCallback(std::string filename, int interval)
    : filename_{std::move(filename)}, interval_{interval} {}

void CheckpointCallback::call(const GenerationStats<double>& stats) {
    if (stats.generation % this->interval_ != 0) {
        return;
    }

    std::ofstream out(this->filename_, std::ios::trunc);

    out << stats.generation << ',' << stats.best_fitness;
    for (double gene : *stats.best_genome) {
        out << ',' << gene;
    }
    out << '\n';
}
