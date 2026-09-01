#include "callbacks/evo_callbacks.hpp"

#include <iostream>

PrintCallback::PrintCallback(PrintField fields) : fields_{fields} {}

void PrintCallback::call(const GenerationStats<double>& stats) {
    bool first = true;
    auto separator = [&first]() {
        if (!first) {
            std::cout << " | ";
        }
        first = false;
    };

    if (has_flag(this->fields_, PrintField::Generation)) {
        separator();
        std::cout << "gen " << stats.generation;
    }
    if (has_flag(this->fields_, PrintField::BestFitness)) {
        separator();
        std::cout << "best fitness " << stats.best_fitness;
    }
    if (has_flag(this->fields_, PrintField::AvgFitness)) {
        separator();
        std::cout << "avg fitness " << stats.avg_fitness;
    }
    if (has_flag(this->fields_, PrintField::Genome)) {
        separator();
        std::cout << "genome:";
        for (double gene : *stats.best_genome) {
            std::cout << ' ' << gene;
        }
    }

    std::cout << '\n';
}
