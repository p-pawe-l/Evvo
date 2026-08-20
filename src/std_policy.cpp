/**
 * @file std_policy.cpp
 * @brief Implements StdPolicy, the default EvoPolicy<double> implementation
 *        used to drive standard genetic-algorithm runs over double genomes.
 */

#include "std_policy.hpp"

#include <random>

std::size_t StdPolicy::roulette_pick(const PopulationEval<double>& eval) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, eval.total_fitness);
    double target = dist(gen);

    double cumulative = 0.0;
    for (std::size_t i = 0; i < eval.fitnesses.size(); ++i) {
        cumulative += eval.fitnesses[i];
        if (cumulative >= target) {
            return i;
        }
    }
    return eval.fitnesses.size() - 1;
}

bool StdPolicy::roll(uint8_t prob) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 255);
    return dist(gen) < prob;
}

std::pair<const IndPtr<double>*, const IndPtr<double>*>
StdPolicy::choose_parents(const PopulationVec<double>& population,
                          const PopulationEval<double>& eval) {
    std::size_t first = roulette_pick(eval);
    std::size_t second = roulette_pick(eval);
    return {&population[first], &population[second]};
}

PopulationVec<double> StdPolicy::create_new_population(const PopulationVec<double>& prev,
                                                       const PopulationEval<double>& eval) {
    PopulationVec<double> new_pop;
    new_pop.reserve(prev.size());
    while (new_pop.size() != prev.size()) {
        auto parents = choose_parents(prev, eval);

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

PopulationEval<double> StdPolicy::evaluate(const PopulationVec<double>& population) {
    PopulationEval<double> eval;
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
