/**
 * @file evo_policy.hpp
 * @brief Defines EvoPolicy<T>, the strategy interface Evolver<T> delegates
 *        to for producing the next generation from the current population.
 */

#pragma once

#include <vector>

#include "genome.hpp"

/**
 * @brief Strategy interface for evolving a population from one generation
 *        to the next (e.g. selection, crossover, mutation policy).
 * @tparam T Gene type of the genomes being evolved.
 */
template <typename T>
class EvoPolicy {
protected:
    double (*eval_func_)(Genome<T>*);

public:
    virtual ~EvoPolicy() = default;

    /**
     * @brief Sets the fitness function used to score genomes.
     * @param eval_func Fitness function used to score genomes.
     */
    void set_eval(double (*eval_func)(Genome<T>*)) {
        this->eval_func_ = eval_func;
    }

    /**
     * @brief Produces the next generation from the current population.
     * @param population Current generation of genomes.
     * @return The new population for the next generation.
     */
    virtual std::vector<Genome<T>*> create_new_population(
        std::vector<Genome<T>*> population) = 0;

    /**
     * @brief Selects the fittest genome in a population.
     * @param population Population to select from.
     * @return The genome with the highest fitness.
     */
    virtual Genome<T>* choose_best(std::vector<Genome<T>*> population) = 0;

    /**
     * @brief Sums fitness over a population.
     * @param population Population to sum fitness over.
     * @return The total fitness of the population.
     */
    virtual double get_total_fitness(std::vector<Genome<T>*> population) = 0;
};
