#pragma once

#include <type_traits>

#include "sampler.hpp"

#include "../util/rand_util.hpp"

namespace evvo::sampling {

template <typename SampleT>
    requires std::is_arithmetic_v<SampleT>
struct RandomSamplerRange {
    SampleT low_;
    SampleT high_;
};

template <typename SampleT>
    requires std::is_arithmetic_v<SampleT>
class RandomSampler : public AbstractSampler<SampleT> {
private:
    evvo::sampling::RandomSamplerRange<SampleT> range_;

public:
    explicit RandomSampler(evvo::sampling::RandomSamplerRange<SampleT>&& range):
        range_{std::move(range)} {}
    RandomSampler(SampleT low, SampleT high): 
        range_{low, high} {}

    [[nodiscard]] SampleT get_sample() const override {
        return random_value<SampleT>(range_.first, range_.second);
    }
};

}