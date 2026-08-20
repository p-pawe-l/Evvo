/**
 * @file evolver.hpp
 * @brief Defines Evolver<T>, which drives a genetic-algorithm run by
 *        repeatedly delegating population evolution to an EvoPolicy<T>.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "genome.hpp"
#include "evo_policy.hpp"
#include "evo_callback.hpp"
#include "generation_stats.hpp"

/**
 * @brief Runs a genetic-algorithm evolution loop over a population of
 *        Genome<T>, using caller-supplied crossover/mutation functions
 *        and an EvoPolicy<T> to produce successive generations. The
 *        initial population is owned by the policy (configured via
 *        EvoPolicy<T>::set_init_population() or set_random_init() before
 *        run() is called).
 * @tparam T Gene type of the genomes being evolved.
 */
template <typename T> class Evolver {
private:
    std::vector<EvoCallback<T>*> callbacks_;
    EvoPolicy<T>* policy_;

public:
    /**
     * @brief Constructs an Evolver with the crossover/mutation functions
     *        and probabilities to use.
     * @param cross_func Function applied to two parent individuals to
     *                   produce a new offspring individual; decides
     *                   itself which genes to combine and how.
     * @param mut_func Function applied to a parent individual to produce
     *                 a new, mutated offspring individual; decides itself
     *                 which genes to change and how.
     * @param callbacks Callbacks invoked, in order, once per generation
     *                  with that generation's GenerationStats<T>.
     * @param policy Policy used to build the initial population, and to
     *               select, score, and evolve populations thereafter;
     *               must have set_init_population()/set_random_init()
     *               called on it before run().
     * @param mutation_prob Probability threshold (0-255) for mutation.
     * @param crossover_prob Probability threshold (0-255) for crossover.
     */
    Evolver(std::function<IndPtr<T>(const IndPtr<T>&, const IndPtr<T>&)> cross_func,
            std::function<IndPtr<T>(const IndPtr<T>&)> mut_func,
            std::vector<EvoCallback<T>*> callbacks, EvoPolicy<T>* policy, uint8_t mutation_prob,
            uint8_t crossover_prob)
        : callbacks_{std::move(callbacks)}, policy_{policy} {
        this->policy_->set_cross_func(cross_func, crossover_prob);
        this->policy_->set_mut_func(mut_func, mutation_prob);
    }

    /**
     * @brief Destroys the Evolver.
     */
    ~Evolver() = default;

    /**
     * @brief Runs the evolution loop for a fixed number of generations.
     * @param runs Number of generations to evolve.
     * @param eval_func Fitness function used to score genomes.
     * @return The best genome found during evolution.
     */
    Genome<T> run(int runs, std::function<double(Genome<T>*)> eval_func) {
        this->policy_->set_eval(eval_func);

        PopulationVec<T> new_pop = this->policy_->create_init_population();
        int generation = 0;

        std::unique_ptr<Genome<T>> best_genome = nullptr;
        double best_fitness = 0.0;

        while (runs > 0) {
            if (new_pop.empty()) {
                break;
            }

            PopulationEval<T> eval = this->policy_->evaluate(new_pop);
            Genome<T>* best = new_pop[eval.best_index].get();

            if (best_genome == nullptr || eval.best_fitness > best_fitness) {
                best_genome = std::make_unique<Genome<T>>(*best);
                best_fitness = eval.best_fitness;
            }

            GenerationStats<T> stats{generation, new_pop, eval.best_fitness,
                                     eval.total_fitness / new_pop.size(), best};
            for (EvoCallback<T>* callback : this->callbacks_) {
                callback->call(stats);
            }

            new_pop = this->policy_->create_new_population(new_pop, eval);
            ++generation;
            runs--;
        }

        return *best_genome;
    }
};
