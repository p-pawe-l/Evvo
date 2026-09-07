#include "../../include/core/elitism_policy.hpp"

#include <stdexcept>
#include <algorithm>
#include <ranges>

ElitismPolicy::ElitismPolicy(std::size_t elite_count): elite_count_{elite_count}, elite_cache_(elite_count_) {}

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
    auto eval_fitnesses_view = eval.fitnesses | std::views::take(eval.fitnesses.size());
    std::ranges::sort(
        eval_fitnesses_view,
        std::ranges::greater{}, 
        &EvalVectorEntry::fitness
    );

    for (const EvalVectorEntry& x : eval_fitnesses_view | std::views::take(elite_count_)) {
        new_pop.push_back(prev[x.org_index]);
    }
}

PopulationVec<double> ElitismPolicy::create_new_population(const PopulationVec<double>& prev, const PopulationEval& eval) { 
}

PopulationEval ElitismPolicy::evaluate(const PopulationVec<double>& population) {
    PopulationEval eval(population.size());
    
    for (const auto& ind : population) {
        auto genome_id = evvo::genome::registry::getid(ind);

        std::optional<genome_entry> cached_val = elite_cache_.get(evvo::genome::registry::getid(ind));
        double ind_fitness;
        if (cached_val) { ind_fitness = cached_val.value() } 
        else { ind_fitness = eval_func_(ind); }
    
        eval.update({genome_id, ind_fitness});
    }

    return eval;
}


