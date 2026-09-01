#include "callbacks/evo_callbacks.hpp"

#include <iostream>

DiversityCallback::DiversityCallback(bool print) : print_{print} {}

void DiversityCallback::call(const GenerationStats<double>& stats) {
    const std::size_t pop_size = stats.population.size();
    const std::size_t genome_len = pop_size > 0 ? stats.population[0].size() : 0;

    double diversity = 0.0;
    for (std::size_t gene = 0; gene < genome_len; ++gene) {
        double mean = 0.0;
        for (const auto& individual : stats.population) {
            mean += individual[gene];
        }
        mean /= static_cast<double>(pop_size);

        double variance = 0.0;
        for (const auto& individual : stats.population) {
            const double diff = individual[gene] - mean;
            variance += diff * diff;
        }
        variance /= static_cast<double>(pop_size);

        diversity += variance;
    }

    this->history_.push_back(diversity);

    if (this->print_) {
        std::cout << "gen " << stats.generation << " diversity " << diversity << '\n';
    }
}

const std::vector<double>& DiversityCallback::history() const {
    return this->history_;
}
