#pragma once

#include <cstddef>
#include <cstdint>

#include "../core/evo_policy.hpp"
#include "../core/genome.hpp"
#include "../util/cache.hpp"

// EvoPolicy<double> that carries the elite_count_ fittest individuals of
// each generation forward unchanged, then fills the rest of the new
// population the same way StdPolicy does (selection, crossover, mutation).
// Prototype: not yet wired into Evolver.
class ElitismPolicy : public EvoPolicy<double> {
private:
    std::size_t elite_count_;
    evvo::cache::fixed_size_cache<std::size_t, Genome<double>> elite_cache_;

    // selector_->build_from_eval() must already have been called for this
    // generation. Returned pointers stay valid only as long as population
    // is alive.
    std::pair<const Genome<double>*, const Genome<double>*>
    choose_parents(const PopulationVec<double>& population);

    // Appends the elite_count_ fittest individuals of prev (ranked via
    // eval.fitnesses) to new_pop.
    void copy_elites(const PopulationVec<double>& prev, const PopulationEval& eval,
                     PopulationVec<double>& new_pop);

    static bool roll(uint8_t prob);

public:
    explicit ElitismPolicy(std::size_t elite_count);
    ~ElitismPolicy() override = default;

    // elite_count must be less than the population size passed to
    // create_new_population().
    void set_elite_count(std::size_t elite_count);

    PopulationVec<double> create_new_population(const PopulationVec<double>& prev,
                                                const PopulationEval& eval) override;

    PopulationEval evaluate(const PopulationVec<double>& population) override;
};
