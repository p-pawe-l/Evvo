/**
 * @file evolver.hpp
 * @brief Defines Evolver<T>, which drives a genetic-algorithm run by
 *        repeatedly delegating population evolution to an EvoPolicy<T>.
 */

#pragma once

#include <cstdint>
#include <vector>

#include "genome.hpp"
#include "evo_policy.hpp"
#include "evo_callback.hpp"
#include "generation_stats.hpp"

/**
 * @brief Runs a genetic-algorithm evolution loop over a population of
 *        Genome<T>, using caller-supplied crossover/mutation functions
 *        and an EvoPolicy<T> to produce successive generations.
 * @tparam T Gene type of the genomes being evolved.
 */
template <typename T>
class Evolver {
private:
    std::vector<Genome<T>*> init_population_;
    uint8_t mutation_prob_;
    uint8_t crossover_prob_;
    uint8_t (*cross_func_)(T*, T*);
    uint8_t (*mut_func_)(T*);

    EvoCallback<T>* callback_;

    Genome<T>* best_genome_;
    EvoPolicy<T>* policy_;


public:
    /**
     * @brief Constructs an Evolver with an initial population and the
     *        crossover/mutation functions and probabilities to use.
     * @param init_population Starting population of genomes.
     * @param cross_func Function applied to two DNA sequences to perform
     *                   crossover.
     * @param mut_func Function applied to a DNA sequence to perform
     *                 mutation.
     * @param callback Callback invoked once per generation with that
     *                 generation's GenerationStats<T>.
     * @param policy Policy used to select, score, and evolve populations.
     * @param mutation_prob Probability threshold (0-255) for mutation.
     * @param crossover_prob Probability threshold (0-255) for crossover.
     */
    Evolver(std::vector<Genome<T>*> init_population,
            uint8_t (*cross_func)(T*, T*),
            uint8_t (*mut_func)(T*),
            EvoCallback<T>* callback,
            EvoPolicy<T>* policy,
            uint8_t mutation_prob,
            uint8_t crossover_prob):
    init_population_{init_population},
    mutation_prob_{mutation_prob},
    crossover_prob_{crossover_prob},
    cross_func_{cross_func},
    mut_func_{mut_func},
    callback_{callback},
    best_genome_{nullptr},
    policy_{policy}
    {}

    /**
     * @brief Destroys the Evolver. Does not own the genomes it references.
     */
    ~Evolver() = default;

    /**
     * @brief Runs the evolution loop for a fixed number of generations.
     * @param runs Number of generations to evolve.
     * @param eval_func Fitness function used to score genomes.
     * @return The best genome found during evolution.
     */
    Genome<T> run(int runs, double (*eval_func)(Genome<T>*)) {
        this->policy_->set_eval(eval_func);

        std::vector<Genome<T>*> new_pop = this->init_population_;
        int generation = 0;

        while(runs > 0) {
            double total_fitness = this->policy_->get_total_fitness(new_pop);

            Genome<T>* best = this->policy_->choose_best(new_pop);
            double best_fitness = eval_func(best);
            if (this->best_genome_ == nullptr || best_fitness > eval_func(this->best_genome_)) {
                this->best_genome_ = best;
            }

            GenerationStats<T> stats{
                generation,
                new_pop,
                best_fitness,
                new_pop.empty() ? 0.0 : total_fitness / new_pop.size(),
                best
            };
            this->callback_->call(stats);

            new_pop = this->policy_->create_new_population(new_pop);
            ++generation;
            runs--;
        }


        return *(this->best_genome_);
    }



};
