#include "callbacks/evo_callbacks.hpp"

void BestGenomeHistoryCallback::call(const GenerationStats<double>& stats) {
    this->history_.push_back(Entry{.generation = stats.generation,
                                   .best_fitness = stats.best_fitness,
                                   .genome = *stats.best_genome});
}

const std::vector<BestGenomeHistoryCallback::Entry>& BestGenomeHistoryCallback::history() const {
    return this->history_;
}
