#include "selection_func.hpp"

#include "rand_util.hpp"
#include <algorithm>

#define SP 1.7

std::size_t roulette_selection(const PopulationEval& eval)
{
    auto rand_val = random_value<double>(0.0, eval.total_fitness);
    double offset = 0.0;
    for (std::size_t i = 0; i < eval.fitnesses.size(); ++i) {
        offset += eval.fitnesses[i];
        if (rand_val <= offset) {
            return i;
        }
    }

    return eval.fitnesses.size() - 1;
}

std::size_t linear_rank_selection(PopulationEval &eval)
{
    std::ranges::sort(eval.fitnesses.begin(), eval.fitnesses.end());
    std::size_t rank;
    std::size_t l = eval.fitnesses.size();
    double prob;
    double offset = 0.0;
    auto random_val = random_value<double>(0.0, 1.0);
    for (std::size_t i = 0; i < l; ++i) {
        rank = i + 1;
        prob = (2 - SP / static_cast<double>(l)) + 
        (2.0 * static_cast<double>(rank) * (SP - 1)) / (static_cast<double>(l) * (l - 1));
    
        offset += prob;
        if (random_val <= offset) {
            return i;
        }
    }

    return eval.fitnesses.size() - 1;
}

std::size_t exp_rank_selection(PopulationEval &eval)
{
    
}
