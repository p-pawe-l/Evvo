#pragma once

#include <cstdint>
#include <utility>

#include "core/evo_policy.hpp"
#include "core/genome.hpp"

// Default EvoPolicy<double>: selects parents via whatever
// Selector<PopulationEval> is configured, applies crossover/mutation.
class StdPolicy : public EvoPolicy<double> {
private:
    // selector_->build_from_eval() must already have been called for this
    // generation. Returned pointers stay valid only as long as population
    // is alive.
    std::pair<const Genome<double>*, const Genome<double>*>
    choose_parents(const PopulationVec<double>& population);

    static bool roll(uint8_t prob);

public:
    StdPolicy() = default;
    ~StdPolicy() override = default;

    PopulationVec<double> create_new_population(const PopulationVec<double>& prev,
                                                const PopulationEval& eval) override;

    PopulationEval evaluate(const PopulationVec<double>& population) override;
};
