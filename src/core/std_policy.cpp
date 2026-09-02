#include "core/std_policy.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>

bool StdPolicy::roll(uint8_t prob) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 255);
    return dist(gen) < prob;
}

std::pair<const Genome<double>*, const Genome<double>*>
StdPolicy::choose_parents(const PopulationVec<double>& population) {
    std::size_t first = selector_->pick();
    std::size_t second = selector_->pick();
    return {&population[first], &population[second]};
}

PopulationVec<double> StdPolicy::create_new_population(const PopulationVec<double>& prev,
                                                       const PopulationEval& eval) {
    assert(selector_ != nullptr && "StdPolicy: call set_selector() before create_new_population()");
    selector_->build_from_eval(eval);

    PopulationVec<double> new_pop;
    new_pop.reserve(prev.size());
    while (new_pop.size() != prev.size()) {
        auto parents = choose_parents(prev);

        Genome<double> offspring =
            roll(crossover_prob_) ? cross_func_(*parents.first, *parents.second) : *parents.first;

        if (roll(mutation_prob_)) {
            offspring = mut_func_(offspring);
        }

        new_pop.push_back(std::move(offspring));
    }
    return new_pop;
}

PopulationEval StdPolicy::evaluate(const PopulationVec<double>& population) {
    PopulationEval eval;
    eval.fitnesses.reserve(population.size());
    for (const Genome<double>& individual : population) {
        eval.fitnesses.push_back(eval_func_(&individual));
    }

    auto best = std::ranges::max_element(eval.fitnesses);
    eval.best_index = static_cast<std::size_t>(std::distance(eval.fitnesses.begin(), best));
    eval.best_fitness = *best;
    eval.total_fitness = std::accumulate(eval.fitnesses.begin(), eval.fitnesses.end(), 0.0);

    return eval;
}
