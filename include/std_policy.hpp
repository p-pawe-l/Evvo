/**
 * @file std_policy.hpp
 * @brief Declares StdPolicy, the default EvoPolicy<double> implementation
 *        used to drive standard genetic-algorithm runs over double genomes.
 *        Implemented in src/std_policy.cpp.
 */

#pragma once

#include <cstdint>
#include <utility>

#include "evo_policy.hpp"
#include "genome.hpp"

/**
 * @brief Default EvoPolicy<double> implementation: selects parents via
 *        fitness-proportionate (roulette wheel) selection, applies
 *        crossover/mutation, and reports fitness over a population of
 *        Genome<double>.
 */
class StdPolicy : public EvoPolicy<double> {
private:
    /**
     * @brief Selects two parent individuals from a population to breed,
     *        biased toward fitter individuals via roulette wheel
     *        selection over eval.fitnesses.
     * @param population Population to select parents from.
     * @param eval Fitnesses of population, as computed by evaluate();
     *             used to bias selection toward fitter individuals.
     * @return Non-owning pointers to the two chosen parents' IndPtr slots
     *         in population; remain valid only as long as population is
     *         alive.
     */
    std::pair<const IndPtr<double>*, const IndPtr<double>*>
    choose_parents(const PopulationVec<double>& population, const PopulationEval<double>& eval);

    /**
     * @brief Picks a single individual via roulette wheel selection,
     *        weighted by eval.fitnesses; assumes every fitness is
     *        non-negative and eval.total_fitness > 0.
     * @param eval Fitnesses to weight selection by.
     * @return Index of the chosen individual.
     */
    static std::size_t roulette_pick(const PopulationEval<double>& eval);

    /**
     * @brief Rolls whether an event with the given probability threshold
     *        occurs.
     * @param prob Probability threshold (0-255); higher means more likely.
     * @return true if the event should occur.
     */
    static bool roll(uint8_t prob);

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
    PopulationVec<double> create_new_population(const PopulationVec<double>& prev,
                                                const PopulationEval<double>& eval) override;

    /**
     * @brief Evaluates every individual in the population exactly once
     *        using eval_func_, collecting every individual's fitness, the
     *        fittest individual's index/fitness, and the population's
     *        total fitness in a single pass.
     * @param population Population to evaluate; must be non-empty.
     * @return The per-individual fitnesses and aggregates found.
     */
    PopulationEval<double> evaluate(const PopulationVec<double>& population) override;
};
