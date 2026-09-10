#pragma once

#include <type_traits>
#include <utility>

#include "evo_policy.hpp"

#include "../core/fitness.hpp"
#include "../core/genome.hpp"
#include "../core/eval.hpp"
#include "../util/cache.hpp"

namespace evvo::policy {

template <typename T,
          typename EvalT,
          evvo::fitness::tactics::concepts::FitnessTactic Tactics = evvo::fitness::tactics::higher_better
>
    requires std::is_default_constructible_v<Tactics>
class ElitismPolicy : public AbstractEvoPolicy<T, EvalT, Tactics> {
    using parents = std::pair<
        const evvo::genome::Genome<T>*,
        const evvo::genome::Genome<T>*
    >;

private:
    std::size_t elite_count_;
    evvo::cache::fixed_size_cache<
        std::size_t, 
        evvo::genome::Genome<T>
    > elite_cache_;

    parents choose_parents(const evvo::genome::Population<T>& population);

    void copy_elites(const evvo::genome::Population<T>& prev, 
                     const evvo::eval::PopulationEval<Tactics>& eval, 
                     evvo::genome::Population<double>& new_pop);

public:
    explicit ElitismPolicy(std::size_t elite_count);
    ~ElitismPolicy() override = default;

    void set_elite_count(std::size_t elite_count);

    evvo::genome::Population<T> create_new_population(const evvo::genome::Population<T>& prev,
                                                      const evvo::eval::PopulationEval<Tactics>& eval) override;

    evvo::eval::PopulationEval<Tactics> evaluate(const evvo::genome::Population<T>& population) override;
};
}