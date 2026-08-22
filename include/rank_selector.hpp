/**
 * @file rank_selector.hpp
 * @brief Defines RankSelector, a linear-rank Selector implementation.
 *        Header-only: k is a non-type template parameter, so every
 *        instantiation's definition must be visible at its call sites.
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <vector>

#include "evo_policy.hpp"
#include "rand_util.hpp"
#include "selector.hpp"

/**
 * @brief Linear rank selector: ranks individuals by fitness once at
 *        construction (worst to best), then picks in O(log n) per pick,
 *        with the ramp between worst and best controlled by k.
 * @tparam k Selection-pressure coefficient in [0, 1]. k=0 gives every
 *           individual an equal, rank-independent chance; k=1 gives the
 *           maximum bias toward higher-ranked (fitter) individuals.
 */
template <double k> class RankSelector : public Selector {
    static_assert(0.0 <= k && k <= 1.0, "Invalid k coefficient for linear rank selection");

private:
    std::vector<std::size_t> order_;      // population indices, ascending by fitness
    std::vector<double> cumulative_;      // cumulative rank probability

public:
    /**
     * @brief Ranks the population by fitness and builds the cumulative
     *        rank-probability array to pick from.
     * @param eval Fitnesses of the population to select from; must have
     *             at least two individuals.
     */
    explicit RankSelector(const PopulationEval& eval) {
        std::size_t n = eval.fitnesses.size();
        order_.resize(n);
        std::iota(order_.begin(), order_.end(), 0);
        std::ranges::sort(order_, {}, [&](std::size_t i) { return eval.fitnesses[i]; });

        double a = (2.0 * k) / (static_cast<double>(n) * static_cast<double>(n - 1));
        double b = (1.0 - k) / static_cast<double>(n);
        cumulative_.resize(n);
        double running = 0.0;
        for (std::size_t rank = 0; rank < n; ++rank) {
            running += a * static_cast<double>(rank) + b;
            cumulative_[rank] = running;
        }
    }

    [[nodiscard]] std::size_t pick() const override {
        auto rand_val = random_value<double>(0.0, 1.0);
        auto it = std::ranges::lower_bound(cumulative_, rand_val);
        std::size_t rank = std::min<std::size_t>(
            static_cast<std::size_t>(std::distance(cumulative_.begin(), it)), order_.size() - 1);
        return order_[rank];
    }
};
