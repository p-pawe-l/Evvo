#include "selection_func.hpp"

#include "rand_util.hpp"

std::size_t roulette_selection(const PopulationEval& eval)
{
    auto rand_val = random_value<double>(0, eval.total_fitness);
    double offset = 0.0;
    for (std::size_t i = 0; i < eval.fitnesses.size(); ++i) {
        offset += eval.fitnesses[i];
        if (rand_val <= offset) {
            return i;
        }
    }

    return eval.fitnesses.size() - 1;
}
