/**
 * @file std_policy.cpp
 * @brief Implements StdPolicy, the default EvoPolicy<double> implementation
 *        used to drive standard genetic-algorithm runs over double genomes.
 */

#include "core/std_policy.hpp"

#include <cassert>
#include <random>

bool StdPolicy::roll(uint8_t prob) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 255);
    return dist(gen) < prob;
}

std::pair<const IndPtr<double>*, const IndPtr<double>*>
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

        IndPtr<double> offspring = roll(crossover_prob_)
                                       ? cross_func_(*parents.first, *parents.second)
                                       : std::make_unique<Genome<double>>(**parents.first);

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

    eval.fitnesses.push_back(eval_func_(population[0].get()));
    eval.best_index = 0;
    eval.best_fitness = eval.fitnesses[0];
    eval.total_fitness = eval.fitnesses[0];

    for (std::size_t i = 1; i < population.size(); ++i) {
        double fitness = eval_func_(population[i].get());
        eval.fitnesses.push_back(fitness);
        eval.total_fitness += fitness;
        if (fitness > eval.best_fitness) {
            eval.best_fitness = fitness;
            eval.best_index = i;
        }
    }

    return eval;
}
