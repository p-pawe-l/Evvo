/**
 * @file evolver.hpp
 * @brief Defines Evolver<T>, which drives a genetic-algorithm run by
 *        repeatedly delegating population evolution to an EvoPolicy<T>.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>

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
    PopulationVec<T> init_population_;
    uint8_t mutation_prob_;
    uint8_t crossover_prob_;
    std::function<IndPtr<T>(const IndPtr<T>&, const IndPtr<T>&)> cross_func_;
    std::function<IndPtr<T>(const IndPtr<T>&)> mut_func_;

    EvoCallback<T>* callback_;

    std::unique_ptr<Genome<T>> best_genome_;
    EvoPolicy<T>* policy_;


public:
    /**
     * @brief Constructs an Evolver with an initial population and the
     *        crossover/mutation functions and probabilities to use.
     * @param init_population Starting population of genomes; ownership is
     *                         transferred into the Evolver.
     * @param cross_func Function applied to two parent individuals to
     *                   produce a new offspring individual; decides
     *                   itself which genes to combine and how.
     * @param mut_func Function applied to a parent individual to produce
     *                 a new, mutated offspring individual; decides itself
     *                 which genes to change and how.
     * @param callback Callback invoked once per generation with that
     *                 generation's GenerationStats<T>.
     * @param policy Policy used to select, score, and evolve populations.
     * @param mutation_prob Probability threshold (0-255) for mutation.
     * @param crossover_prob Probability threshold (0-255) for crossover.
     */
    Evolver(PopulationVec<T> init_population,
            std::function<IndPtr<T>(const IndPtr<T>&, const IndPtr<T>&)> cross_func,
            std::function<IndPtr<T>(const IndPtr<T>&)> mut_func,
            EvoCallback<T>* callback,
            EvoPolicy<T>* policy,
            uint8_t mutation_prob,
            uint8_t crossover_prob):
    init_population_{std::move(init_population)},
    mutation_prob_{mutation_prob},
    crossover_prob_{crossover_prob},
    cross_func_{cross_func},
    mut_func_{mut_func},
    callback_{callback},
    best_genome_{nullptr},
    policy_{policy}
    {}

    /**
     * @brief Destroys the Evolver, freeing the population and best genome
     *        it owns.
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
        this->policy_->set_cross_func(cross_func_, crossover_prob_);
        this->policy_->set_mut_func(mut_func_, mutation_prob_);

        PopulationVec<T> new_pop = std::move(this->init_population_);
        int generation = 0;

        while(runs > 0) {
            double total_fitness = this->policy_->get_total_fitness(new_pop);

            Genome<T>* best = this->policy_->choose_best(new_pop);
            double best_fitness = eval_func(best);
            if (this->best_genome_ == nullptr || best_fitness > eval_func(this->best_genome_.get())) {
                this->best_genome_ = std::make_unique<Genome<T>>(*best);
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
