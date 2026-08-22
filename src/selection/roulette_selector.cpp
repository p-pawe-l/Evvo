#include "../../include/selection/roulette_selector.hpp"
#include "../../include/util/rand_util.hpp"

void RouletteSelector::build_from_eval(const PopulationEval& eval) {
    total_fitness_ = eval.total_fitness;
    fitnesses_ = &eval.fitnesses;
}

std::size_t RouletteSelector::pick() const 
{
    auto rand_val = random_value<double>(0.0, total_fitness_);
    std::size_t n = fitnesses_->size();
    double offset = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        offset += (*fitnesses_)[i];
        if (rand_val <= offset) {
            return i;
        }
    }
    return (n - 1);
}
