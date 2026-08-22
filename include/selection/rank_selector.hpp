/**
 * @file rank_selector.hpp
 * @brief Declares RankSelector, a rank-based Selector implementation.
 *        Implemented in src/selection/rank_selector.cpp.
 */

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

/**
 * @brief Rank selector: ranks individuals by fitness in build_from_eval()
 *        (worst to best), then picks in O(log n) per pick, with the ramp
 *        between worst and best shaped by type and controlled by k.
 */
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
    std::vector<double> cumulative_;   // cumulative rank probability, parallel to order_

public:
    /**
     * @brief Configures the rank-selection shape.
     * @param type Whether the rank-to-probability ramp is linear or
     *             exponential.
     * @param k Selection-pressure coefficient in [0, 1]. k=0 gives every
     *          individual an equal, rank-independent chance; k=1 gives
     *          the maximum bias toward higher-ranked (fitter)
     *          individuals.
     */
    explicit RankSelector(RankSelectionType type, double k);

    /**
     * @brief Ranks the population by fitness and builds the cumulative
     *        rank-probability array to pick from.
     * @param eval Fitnesses of the population to select from; must have
     *             at least two individuals.
     */
    void build_from_eval(const PopulationEval& eval) override;

    [[nodiscard]] std::size_t pick() const override;
};
