#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <vector>

#include "selector.hpp"

#include "../core/fitness.hpp"
#include "../core/eval.hpp"

namespace evvo::selection {

enum class RankSelectionType : std::uint8_t {
    LINEAR = 1,
    EXPONENTIAL = 2,
};

template <evvo::fitness::tactics::concepts::FitnessTactic Tactics>
    requires std::is_default_constructible_v<Tactics>
class RankSelector : public evvo::selection::AbstractSelector<evvo::eval::PopulationEval<Tactics>> {
private:
    struct IndexWithFit {
        std::size_t orginal_index_;
        double fitness_;
    };

    std::function<double(std::size_t rank, std::size_t n)> weight_fn_;
    std::vector<IndexWithFit> order_;
    std::vector<double> cumulative_;

public:
    explicit RankSelector(RankSelectionType type, double k);
    void build_from_eval(const evvo::eval::PopulationEval<Tactics>& eval) override;
    [[nodiscard]] std::size_t pick() const override;
};

}