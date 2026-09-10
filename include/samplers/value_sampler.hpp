#pragma once

#include "sampler.hpp"

namespace evvo::sampling {

template <typename SampleT> 
class ValueSampler : public evvo::sampling::AbstractSampler<SampleT> {
private:
    SampleT val_;

public:
    explicit ValueSampler(SampleT value): 
        val_{value} {}

    [[nodiscard]] SampleT get_sample() const override { return val_; }
};

}
