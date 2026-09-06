#include "../../include/core/elitism_policy.hpp"

#include <stdexcept>
#include <algorithm>
#include <ranges>

ElitismPolicy::ElitismPolicy(const std::size_t elite_count):
    elite_count_{elite_count}
{}

void ElitismPolicy::set_elite_count(const std::size_t elite_count) {
    if (elite_count <= 0) {
        throw std::invalid_argument("Elite count must be greather than 0");
    }
    elite_count_ = elite_count;
}

std::pair<const Genome<double>*, const Genome<double>*> ElitismPolicy::choose_parents(const PopulationVec<double>& population) {
    auto first_index = selector_->pick();
    auto second_index = selector_->pick();

    return {&population[first_index], &population[second_index]};
}

void ElitismPolicy::copy_elites(const PopulationVec<double>& prev, const PopulationEval& eval, PopulationVec<double>& new_pop) {
    if (elite_count_ <= 0 || elite_count_ >= eval.fitnesses.size()) {
        throw std::invalid_argument("Elite count must be between 0 and population size.");
    }
    
    // Maybe better solution than just copying copy-demanding vector ?
    std::vector<EvalVectorEntry> eval_fitnesses_copy = eval.fitnesses;
    std::ranges::sort(
            eval_fitnesses_copy,
            std::ranges::greater{},
            &EvalVectorEntry::fitness
    );

    for (const EvalVectorEntry& x : eval_fitnesses_copy | std::views::take(elite_count_)) {
        new_pop.push_back(prev[x.org_index]);
    }
}


