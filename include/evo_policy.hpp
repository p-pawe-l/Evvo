/**
 * @file evo_policy.hpp
 * @brief Defines EvoPolicy<T>, the strategy interface Evolver<T> delegates
 *        to for producing the next generation from the current population.
 */

#pragma once

#include <cstddef>
#include <functional>
#include <vector>

#include "genome.hpp"

/**
 * @brief Result of a single pass over a population evaluating every
 *        individual's fitness exactly once. Stores each individual's
 *        fitness (indexed the same as the population it was computed
 *        from) alongside the aggregates, so later steps (e.g. parent
 *        selection) can reuse it instead of calling eval_func_ again.
 * @tparam T Gene type of the genomes that were evaluated.
 */
template <typename T>
struct PopulationEval {
    std::vector<double> fitnesses;
    std::size_t best_index;
    double best_fitness;
    double total_fitness;
};

/**
 * @brief Strategy interface for evolving a population from one generation
 *        to the next (e.g. selection, crossover, mutation policy).
 * @tparam T Gene type of the genomes being evolved.
 */
template <typename T>
class EvoPolicy {
protected:
    std::function<double(Genome<T>*)> eval_func_;
    std::function<IndPtr<T>(const IndPtr<T>&, const IndPtr<T>&)> cross_func_;
    std::function<IndPtr<T>(const IndPtr<T>&)> mut_func_;
    uint8_t crossover_prob_;
    uint8_t mutation_prob_;

public:
    virtual ~EvoPolicy() = default;

    /**
     * @brief Sets the fitness function used to score genomes.
     * @param eval_func Fitness function used to score genomes.
     */
    void set_eval(std::function<double(Genome<T>*)> eval_func) {
        this->eval_func_ = eval_func;
    }

    /**
     * @brief Sets the crossover function and the probability with which it
     *        is applied to two parent individuals during reproduction.
     * @param cross_func Function applied to two parent individuals to
     *                    produce a new offspring individual; decides
     *                    itself which genes to combine and how.
     * @param crossover_prob Probability threshold (0-255) for crossover.
     */
    void set_cross_func(std::function<IndPtr<T>(const IndPtr<T>&, const IndPtr<T>&)> cross_func, uint8_t crossover_prob) {
        this->cross_func_ = cross_func;
        this->crossover_prob_ = crossover_prob;
    }

    /**
     * @brief Sets the mutation function and the probability with which it
     *        is applied to an individual during reproduction.
     * @param mut_func Function applied to a parent individual to produce
     *                 a new, mutated offspring individual; decides itself
     *                 which genes to change and how.
     * @param mutation_prob Probability threshold (0-255) for mutation.
     */
    void set_mut_func(std::function<IndPtr<T>(const IndPtr<T>&)> mut_func, uint8_t mutation_prob) {
        this->mut_func_ = mut_func;
        this->mutation_prob_ = mutation_prob;
    }

    /**
     * @brief Produces the next generation from the current population.
     * @param population Current generation of genomes.
     * @param eval Fitnesses of population, as computed by evaluate();
     *             reused for parent selection instead of re-evaluating.
     * @return The new, owned population for the next generation.
     */
    virtual PopulationVec<T> create_new_population(
        const PopulationVec<T>& population, const PopulationEval<T>& eval) = 0;

    /**
     * @brief Evaluates every individual in a population exactly once,
     *        collecting every individual's fitness, the fittest
     *        individual's index/fitness, and the population's total
     *        fitness in a single pass.
     * @param population Population to evaluate; must be non-empty.
     * @return The per-individual fitnesses and aggregates found.
     */
    virtual PopulationEval<T> evaluate(const PopulationVec<T>& population) = 0;
};
