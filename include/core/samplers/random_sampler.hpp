#pragma once

#include <random>
#include <type_traits>

#include "../../util/rand_util.hpp"
#include "../population_init.hpp"

template <typename SampleT>
    requires std::is_arithmetic_v<SampleT>
struct RandomSamplerParams {
    SampleT low_;
    SampleT high_;
};

template <typename SampleT>
    requires std::is_arithmetic_v<SampleT>
class RandomSampler : public Sampler<SampleT> {
private:
    std::pair<SampleT, SampleT> range_;

public:
    explicit RandomSampler(const RandomSamplerParams<SampleT>& params)
        : range_{params.low_, params.high_} {}
    RandomSampler(SampleT low, SampleT high) : range_{low, high} {}

    [[nodiscard]] SampleT get_sample() const noexcept override {
        return random_value<SampleT>(range_.first, range_.second);
    }
};
