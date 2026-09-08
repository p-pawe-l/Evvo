#pragma once

#include <utility>
#include <type_traits>

#include "evo_policy.hpp"

#include "../core/genome.hpp"
#include "../core/fitness.hpp"
#include "../util/rand_util.hpp"

namespace evvo::policy {
template <typename T,
          typename EvalT,
          evvo::fitness::tactics::concepts::FitnessTactic Tactics = evvo::fitness::tactics::higher_better
> requires std::is_default_constructible_v<Tactics>
class StdPolicy : public AbstractEvoPolicy<T, EvalT, Tactics> {
    using parents = std::pair<
        const evvo::genome::Genome<T>*,
        const evvo::genome::Genome<T>*
    >;

    using f32 = float;

private:    
    parents choose_parents(const evvo::genome::Population<T>& population) const {
        // Selector class here handles picking parents from population
        // by given selector policy
        return {
            &population[this->selector_->pick()], // first parent
            &population[this->selector_->pick()]  // second parent
        };
    }

public:
    explicit StdPolicy(evvo::policy::Probabilities probs):
        evvo::policy::AbstractEvoPolicy<T, EvalT, Tactics>(std::move(probs)) {}
    ~StdPolicy() override = default;

    evvo::genome::Population<T> create_new_population(
        const evvo::genome::Population<T>& prev,
        const evvo::eval::PopulationEval<Tactics>& eval
    ) override {
        evvo::genome::Population<T> new_population;
        this->selector_->build_from_eval(eval);
        
        while (new_population.size() != prev.size()) {
            parents pair = choose_parents(prev);
            evvo::genome::Genome<T> offspring = pair.first;
            if (random_value<f32>(0.00, 1.00) < this->crossover_chance_) {
                offspring = this->crossover_func_(*(pair.first), *(pair.second));
            }
            if (random_value<f32>(0.00, 1.00) < this->mutation_chance_) {
                offspring = this->mutating_func_(offspring);
            }
            new_population.push_back(std::move(offspring));
        }
        return new_population;
    } 

    evvo::eval::PopulationEval<Tactics> evaluate(
        const evvo::genome::Population<T>& population
    ) override {
        evvo::eval::PopulationEval<Tactics> population_eval; 
        for (std::size_t i = 0; i < population.size(); ++i) {
            evvo::eval::GenomeEntry entry = {
                .id = evvo::genome::get_current_id(), // somehow this should be implemented to give each genome an unique id
                .population_index = i,
                .fitness = this->evaluation_(population[i])
            }
            population_eval.update(std::move(entry));
        }
        return population_eval;
    }
};
}