#pragma once

#include <cstddef>

#include "../core/genome.hpp"

// Polymorphic alternative to passing a generator callable, so a sampling
// strategy can be swapped at runtime (or hold state, e.g. a distribution
// object) instead of being fixed at compile time via a template parameter.
template <typename SampleT> class Sampler {
public:
    virtual ~Sampler() = default;

    [[nodiscard]] virtual SampleT get_sample() const noexcept = 0;
};

struct InitPopSizeParams {
    const std::size_t population_size_;
    const std::size_t genome_len_;
};

template <typename T>
PopulationVec<T> generate_population(const InitPopSizeParams& params, const Sampler<T>& gene_gen) {
    PopulationVec<T> population;
    population.reserve(params.population_size_);
    for (std::size_t i = 0; i < params.population_size_; ++i) {
        population.push_back(
            make_genome<T>(params.genome_len_, [&gene_gen]() { return gene_gen.get_sample(); }));
    }
    return population;
}
