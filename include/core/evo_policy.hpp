#pragma once

#include <cassert>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

#include "../core/genome.hpp"
#include "../core/population_init.hpp"
#include "../core/samplers/random_sampler.hpp"
#include "../selection/selector.hpp"

// Caches a population's per-individual fitnesses alongside the aggregates
// so later steps (e.g. parent selection) can reuse them instead of calling
// eval_func_ again.
struct PopulationEval {
    std::vector<double> fitnesses;
    std::size_t best_index;
    double best_fitness;
    double total_fitness;
};

// Strategy interface Evolver<T> delegates to for producing the next
// generation from the current population (selection, crossover, mutation).
template <typename T> class EvoPolicy {
protected:
    std::function<double(const Genome<T>*)> eval_func_;
    std::function<Genome<T>(const Genome<T>&, const Genome<T>&)> cross_func_;
    std::function<Genome<T>(const Genome<T>&)> mut_func_;
    uint8_t crossover_prob_;
    uint8_t mutation_prob_;

    // Non-owning: lifetime is managed by whoever configures this policy
    // (see Evolver<T>, which owns the concrete Selector it constructs
    // and outlives every create_new_population() call it's used in).
    Selector<PopulationEval>* selector_ = nullptr;

    PopulationVec<T> init_population_;
    bool has_init_population_ = false;

public:
    virtual ~EvoPolicy() = default;

    void set_init_population(PopulationVec<T> population) {
        this->init_population_ = std::move(population);
        this->has_init_population_ = true;
    }

    // Draws each gene uniformly from [lo, hi]; deduces an integer or real
    // distribution from T (see RandomSampler in random_sampler.hpp).
    void set_random_init(std::size_t population_size, std::size_t genome_len, T lo, T hi) {
        RandomSampler<T> sampler(lo, hi);
        this->init_population_ =
            generate_population<T>(InitPopSizeParams{population_size, genome_len}, sampler);
        this->has_init_population_ = true;
    }

    // Requires set_init_population() or set_random_init() to have been
    // called first.
    PopulationVec<T> create_init_population() {
        assert(this->has_init_population_ && "EvoPolicy: call set_init_population() or "
                                             "set_random_init() before create_init_population()");
        this->has_init_population_ = false;
        return std::move(this->init_population_);
    }

    void set_eval(std::function<double(const Genome<T>*)> eval_func) {
        this->eval_func_ = std::move(eval_func);
    }

    void set_cross_func(std::function<Genome<T>(const Genome<T>&, const Genome<T>&)> cross_func,
                        uint8_t crossover_prob) {
        this->cross_func_ = std::move(cross_func);
        this->crossover_prob_ = crossover_prob;
    }

    void set_mut_func(std::function<Genome<T>(const Genome<T>&)> mut_func, uint8_t mutation_prob) {
        this->mut_func_ = std::move(mut_func);
        this->mutation_prob_ = mutation_prob;
    }

    // selector must outlive every create_new_population() call made on
    // this policy.
    void set_selector(Selector<PopulationEval>* selector) { this->selector_ = selector; }

    virtual PopulationVec<T> create_new_population(const PopulationVec<T>& population,
                                                   const PopulationEval& eval) = 0;

    // population must be non-empty.
    virtual PopulationEval evaluate(const PopulationVec<T>& population) = 0;
};
