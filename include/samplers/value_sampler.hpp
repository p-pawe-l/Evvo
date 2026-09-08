#pragma once

#include "../population_init.hpp"

template <typename SampleT> class ValueSampler : public Sampler<SampleT> {
private:
    SampleT val_;

public:
    explicit ValueSampler(SampleT value) : val_{value} {}

    [[nodiscard]] SampleT get_sample() const noexcept override { return val_; }
};
