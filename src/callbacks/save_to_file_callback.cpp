#include "callbacks/evo_callbacks.hpp"

#include <fstream>
#include <utility>

SaveToFileCallback::SaveToFileCallback(std::string filename) : filename_{std::move(filename)} {
    // Truncate any previous contents so each run starts a fresh file.
    std::ofstream out(this->filename_, std::ios::trunc);
}

void SaveToFileCallback::call(const GenerationStats<double>& stats) {
    std::ofstream out(this->filename_, std::ios::app);

    out << stats.generation << ',' << stats.best_fitness << ',' << stats.avg_fitness;
    for (double gene : *stats.best_genome) {
        out << ',' << gene;
    }
    out << '\n';
}
