#pragma once

#include <type_traits>

#include "../../util/rand_util.hpp"
#include "../population_init.hpp"

template <typename SampleT>
    requires std::is_arithmetic_v<SampleT>
class GaussianSampler : public Sampler<SampleT> {
private:
    double mean_;
    double std_dev_;

public:
    GaussianSampler(double mean, double std_dev) : mean_{mean}, std_dev_{std_dev} {}

    [[nodiscard]] SampleT get_sample() const noexcept override {
        return static_cast<SampleT>(random_gaussian(mean_, std_dev_));
    }
};
