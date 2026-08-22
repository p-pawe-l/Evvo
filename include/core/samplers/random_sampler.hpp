#pragma once

#include "../population_init.hpp"
#include "../../util/rand_util.hpp"
#include <type_traits>  
#include <random>

template <typename SampleT> requires std::is_arithmetic_v<SampleT>
struct RandomSamplerParams {
    SampleT low_;
    SampleT high_;
};

template <typename SampleT> requires std::is_arithmetic_v<SampleT>
class RandomSampler : public Sampler<SampleT> {
private:
    std::pair<SampleT, SampleT> range_;

public:
    explicit RandomSampler(const RandomSamplerParams& params): range_{params.low_, params.high_} {}
    RandomSampler(SampleT low, SampleT high): range_{low, high} {}

    SampleT get_sample() const override {
        SampleT rand_val = random_value<SampleT>(range_.first, range_.second);
        return rand_val;
    }

};
