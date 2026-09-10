#pragma once

#include <type_traits>

#include "sampler.hpp"

#include "../util/rand_util.hpp"

namespace evvo::sampling {

struct GaussianSamplersMeta {
    double mean;
    double std_dev;
};

template <typename SampleT>
    requires std::is_arithmetic_v<SampleT>
class GaussianSampler : public evvo::sampling::AbstractSampler<SampleT> {
private:
    double mean_;
    double std_dev_;

public:
    explicit GaussianSampler(evvo::sampling::GaussianSamplersMeta&& gaussian_meta): 
        mean_{gaussian_meta.mean}, std_dev_{gaussian_meta.std_dev} {}

    [[nodiscard]] SampleT get_sample() const override {
        return static_cast<SampleT>(random_gaussian(mean_, std_dev_));
    }
};

}
