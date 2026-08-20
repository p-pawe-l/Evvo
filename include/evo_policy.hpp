/**
 * @file evo_policy.hpp
 * @brief Defines EvoPolicy<T>, the strategy interface Evolver<T> delegates
 *        to for producing the next generation from the current population.
 */

#pragma once

#include <cassert>
#include <cstddef>
#include <functional>
#include <vector>

#include "genome.hpp"
#include "population_init.hpp"
#include "rand_util.hpp"

/**
 * @brief Result of a single pass over a population evaluating every
 *        individual's fitness exactly once. Stores each individual's
 *        fitness (indexed the same as the population it was computed
 *        from) alongside the aggregates, so later steps (e.g. parent
 *        selection) can reuse it instead of calling eval_func_ again.
 * @tparam T Gene type of the genomes that were evaluated.
 */
template <typename T> struct PopulationEval {
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
template <typename T> class EvoPolicy {
protected:
    std::function<double(Genome<T>*)> eval_func_;
    std::function<IndPtr<T>(const IndPtr<T>&, const IndPtr<T>&)> cross_func_;
    std::function<IndPtr<T>(const IndPtr<T>&)> mut_func_;
    uint8_t crossover_prob_;
    uint8_t mutation_prob_;

    PopulationVec<T> init_population_;
    bool has_init_population_ = false;

public:
    virtual ~EvoPolicy() = default;

    /**
     * @brief Sets the population create_init_population() will hand back,
     *        moved in as-is.
     * @param population Initial population to use; ownership is
     *                    transferred into the policy.
     */
    void set_init_population(PopulationVec<T> population) {
        this->init_population_ = std::move(population);
        this->has_init_population_ = true;
    }

    /**
     * @brief Configures create_init_population() to build a fresh
     *        population of population_size genomes, each with genome_len
     *        genes drawn uniformly from [lo, hi]. Whether that draw uses
     *        an integer or real distribution is deduced from T (see
     *        random_value in rand_util.hpp).
     * @param population_size Number of individuals to generate.
     * @param genome_len Number of genes per individual.
     * @param lo Lower bound (inclusive) for each gene.
     * @param hi Upper bound (inclusive) for each gene.
     */
    void set_random_init(std::size_t population_size, std::size_t genome_len, T lo, T hi) {
        this->init_population_ = make_random_population<T>(
            population_size, genome_len, [lo, hi]() { return random_value<T>(lo, hi); });
        this->has_init_population_ = true;
    }

    /**
     * @brief Hands back the population configured via
     *        set_init_population() or set_random_init(); one of those
     *        must have been called first.
     * @return The initial population, moved out of the policy.
     */
    PopulationVec<T> create_init_population() {
        assert(this->has_init_population_ && "EvoPolicy: call set_init_population() or "
                                             "set_random_init() before create_init_population()");
        this->has_init_population_ = false;
        return std::move(this->init_population_);
    }

    /**
     * @brief Sets the fitness function used to score genomes.
     * @param eval_func Fitness function used to score genomes.
     */
    void set_eval(std::function<double(Genome<T>*)> eval_func) { this->eval_func_ = eval_func; }

    /**
     * @brief Sets the crossover function and the probability with which it
     *        is applied to two parent individuals during reproduction.
     * @param cross_func Function applied to two parent individuals to
     *                    produce a new offspring individual; decides
     *                    itself which genes to combine and how.
     * @param crossover_prob Probability threshold (0-255) for crossover.
     */
    void set_cross_func(std::function<IndPtr<T>(const IndPtr<T>&, const IndPtr<T>&)> cross_func,
                        uint8_t crossover_prob) {
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
    virtual PopulationVec<T> create_new_population(const PopulationVec<T>& population,
                                                   const PopulationEval<T>& eval) = 0;

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
