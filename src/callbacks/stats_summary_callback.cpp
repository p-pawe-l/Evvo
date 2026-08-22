/**
 * @file stats_summary_callback.cpp
 * @brief Implements StatsSummaryCallback.
 */

#include "callbacks/evo_callbacks.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <utility>

StatsSummaryCallback::StatsSummaryCallback(std::string filename) : filename_{std::move(filename)} {
    std::ofstream out(this->filename_, std::ios::trunc);
}

void StatsSummaryCallback::call(const GenerationStats<double>& stats) {
    std::ofstream out(this->filename_, std::ios::app);

    const std::vector<double>& fitnesses = stats.fitnesses;
    double min_fitness = fitnesses.empty() ? 0.0 : fitnesses[0];
    double max_fitness = fitnesses.empty() ? 0.0 : fitnesses[0];
    for (double fitness : fitnesses) {
        min_fitness = std::min(min_fitness, fitness);
        max_fitness = std::max(max_fitness, fitness);
    }

    double variance = 0.0;
    for (double fitness : fitnesses) {
        const double diff = fitness - stats.avg_fitness;
        variance += diff * diff;
    }
    variance = fitnesses.empty() ? 0.0 : variance / static_cast<double>(fitnesses.size());
    const double stddev = std::sqrt(variance);

    out << stats.generation << ',' << stats.best_fitness << ',' << stats.avg_fitness << ','
        << min_fitness << ',' << max_fitness << ',' << stddev << '\n';
}
