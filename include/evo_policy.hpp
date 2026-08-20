/**
 * @file evo_policy.hpp
 * @brief Defines EvoPolicy<T>, the strategy interface Evolver<T> delegates
 *        to for producing the next generation from the current population.
 */

#pragma once

#include <functional>

#include "genome.hpp"

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
     * @return The new, owned population for the next generation.
     */
    virtual PopulationVec<T> create_new_population(
        const PopulationVec<T>& population) = 0;

    /**
     * @brief Selects the fittest genome in a population.
     * @param population Population to select from.
     * @return Non-owning pointer to the genome with the highest fitness;
     *         remains valid only as long as population is alive.
     */
    virtual Genome<T>* choose_best(const PopulationVec<T>& population) = 0;

    /**
     * @brief Sums fitness over a population.
     * @param population Population to sum fitness over.
     * @return The total fitness of the population.
     */
    virtual double get_total_fitness(const PopulationVec<T>& population) = 0;
};
