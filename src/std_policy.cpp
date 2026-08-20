/**
 * @file std_policy.cpp
 * @brief Defines StdPolicy, the default EvoPolicy<double> implementation
 *        used to drive standard genetic-algorithm runs over double genomes.
 */

#include <random>

#include "../include/evo_policy.hpp"
#include "genome.hpp"


/**
 * @brief Default EvoPolicy<double> implementation: selects parents,
 *        applies crossover/mutation, and reports fitness over a
 *        population of Genome<double>.
 */
class StdPolicy : public EvoPolicy<double> {
private:
    /**
     * @brief Selects two parent individuals from a population to breed.
     * @param population Population to select parents from.
     * @param eval Fitnesses of population, as computed by evaluate();
     *             used to bias selection toward fitter individuals.
     * @return Non-owning pointers to the two chosen parents' IndPtr slots
     *         in population; remain valid only as long as population is
     *         alive.
     */
    std::pair<const IndPtr<double>*, const IndPtr<double>*> choose_parents(
        const PopulationVec<double>& population, const PopulationEval<double>& eval
    )
    {
        return {nullptr, nullptr};
    }

    /**
     * @brief Rolls whether an event with the given probability threshold
     *        occurs.
     * @param prob Probability threshold (0-255); higher means more likely.
     * @return true if the event should occur.
     */
    static bool roll(uint8_t prob) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dist(0, 255);
        return dist(gen) < prob;
    }

public:
    StdPolicy() = default;
    ~StdPolicy() = default;


    /**
     * @brief Produces the next generation by repeatedly selecting parents
     *        and applying the configured crossover/mutation functions.
     * @param prev Current generation of genomes.
     * @param eval Fitnesses of prev, as computed by evaluate(); reused for
     *             parent selection instead of re-evaluating.
     * @return The new, owned population for the next generation.
     */
    PopulationVec<double> create_new_population(
        const PopulationVec<double>& prev, const PopulationEval<double>& eval) override
    {
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

    /**
     * @brief Evaluates every individual in the population exactly once
     *        using eval_func_, collecting every individual's fitness, the
     *        fittest individual's index/fitness, and the population's
     *        total fitness in a single pass.
     * @param population Population to evaluate; must be non-empty.
     * @return The per-individual fitnesses and aggregates found.
     */
    PopulationEval<double> evaluate(const PopulationVec<double>& population) override
    {
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
};