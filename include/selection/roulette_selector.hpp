#pragma once

#include "selector.hpp"

#include "../policy/evo_policy.hpp"
#include "../core/fitness.hpp"
#include "../core/eval.hpp"

namespace evvo::selection {

template <evvo::fitness::tactics::concepts::FitnessTactic Tactics>
    requires std::is_default_constructible_v<Tactics>
class RouletteSelector : public evvo::selection::AbstractSelector<evvo::eval::PopulationEval<Tactics>> {
private:
    double total_fitness_;
    const std::vector<double>* fitnesses_;

public:
    void build_from_eval(const evvo::eval::PopulationEval<Tactics>& eval) override;

    [[nodiscard]] std::size_t pick() const override;
};

}