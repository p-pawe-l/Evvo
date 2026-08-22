#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>

#include "../../include/selection/rank_selector.hpp"
#include "../../include/util/rand_util.hpp"


RankSelector::RankSelector(RankSelectionType type, double k)
{
    assert(k >= 0.0 && k <= 1.0 && "Invalid coefficient for rank selection!");

    if (type == RankSelectionType::LINEAR) {
        weight_fn_ = [k](std::size_t rank, std::size_t n) {
            auto N = static_cast<double>(n);
            return (((2.0 * k) / (N * (N - 1.0))) * static_cast<double>(rank)) + ((1.0 - k) / N);
        };
    } else {
        weight_fn_ = [k](std::size_t rank, std::size_t n) {
            auto N = static_cast<double>(n);
            double c = 1.0 - k;   // c=1 (k=0) -> uniform; c=0 (k=1) -> all weight on the fittest
            return std::pow(c, N - 1.0 - static_cast<double>(rank));
        };
    }
}

void RankSelector::build_from_eval(const PopulationEval& eval)
{
    std::size_t n = eval.fitnesses.size();
    order_.resize(n);
    for (std::size_t i = 0; i < n; ++i) {
        order_.emplace_back(i, eval.fitnesses[i]);
    }
    std::ranges::sort(order_, {}, [](const IndexWithFit& iwf) { return iwf.fitness_; });

    std::vector<double> weights(n);
    double total = 0.0;
    for (std::size_t rank = 0; rank < n; ++rank) {
        weights[rank] = weight_fn_(rank, n);
        total += weights[rank];
    }

    cumulative_.resize(n);
    double running = 0.0;
    for (std::size_t rank = 0; rank < n; ++rank) {
        running += weights[rank] / total;
        cumulative_[rank] = running;
    }
}

std::size_t RankSelector::pick() const
{
    auto rand_val = random_value<double>(0.0, 1.0);
    auto it = std::ranges::lower_bound(cumulative_, rand_val);
    std::size_t rank = std::min<std::size_t>(
        static_cast<std::size_t>(std::distance(cumulative_.begin(), it)), order_.size() - 1);
    return order_[rank].orginal_index_;
}