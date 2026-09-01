#pragma once

#include <cstddef>
#include <functional>
#include <vector>

#include "../core/evo_policy.hpp"
#include "selector.hpp"

enum class RankSelectionType : uint8_t {
    LINEAR = 1,
    EXPONENTIAL = 2,
};

// Ranks individuals by fitness in build_from_eval() (worst to best), then
// picks in O(log n) per pick, with the ramp between worst and best shaped
// by type and controlled by k.
class RankSelector : public Selector<PopulationEval> {
private:
    struct IndexWithFit {
        std::size_t orginal_index_;
        double fitness_;
    };

    // Raw (unnormalized) weight for a rank in [0, n); build_from_eval()
    // normalizes the weights it returns across the whole population.
    std::function<double(std::size_t rank, std::size_t n)> weight_fn_;

    std::vector<IndexWithFit> order_;
    std::vector<double> cumulative_; // cumulative rank probability, parallel to order_

public:
    // k is the selection-pressure coefficient in [0, 1]: k=0 gives every
    // individual an equal, rank-independent chance; k=1 gives the maximum
    // bias toward higher-ranked (fitter) individuals.
    explicit RankSelector(RankSelectionType type, double k);

    // eval must have at least two individuals.
    void build_from_eval(const PopulationEval& eval) override;

    [[nodiscard]] std::size_t pick() const override;
};
