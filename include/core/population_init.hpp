/**
 * @file population_init.hpp
 * @brief Helper for building an initial PopulationVec<T> without hand-
 *        rolling the same generation loop at every call site.
 */

#pragma once

#include <cstddef>

#include "../core/genome.hpp"
#include "../core/types.hpp"

/**
 * @brief Interface for a gene-value source: a polymorphic alternative to
 *        passing a generator callable, so a sampling strategy can be
 *        swapped at runtime (or hold state, e.g. a distribution object)
 *        instead of being fixed at compile time via a template parameter.
 * @tparam SampleT Type of value produced by get_sample(), e.g. the gene
 *                 type T a genome is built from.
 */
template <typename SampleT>
class Sampler {
public:
    virtual ~Sampler() = default;

    /**
     * @brief Produces one sampled value.
     * @return The sampled value.
     */
    virtual SampleT get_sample() const noexcept = 0;
};

/**
 * @brief Parameters for make_random_population function
 * @param population_size_ Size of initial population
 * @param genome_len_ Length of each individual genome
 */
struct InitPopSizeParams {
    const std::size_t population_size_;
    const std::size_t genome_len_;
};

/**
 * @brief Builds a population of population_size genomes, each with
 *        genome_len genes produced by invoking gene_gen once per gene.
 * @tparam T Gene type of the genomes to build.
 * @tparam Generator Callable with signature T(), e.g. a random-value
 *                    generator; called population_size * genome_len times.
 * @param params Structurized parameters for init population building
 * @param gene_gen Invoked once per gene to produce its value.
 * @return The newly built population.
 */
template <typename T>
PopulationVec<T> generate_population(const InitPopSizeParams& params, const Sampler<T>& gene_gen) {
    PopulationVec<T> population;
    population.reserve(params.population_size_);
    for (std::size_t i = 0; i < params.population_size_; ++i) {
        population.push_back(
            std::make_unique<Genome<T>>(make_genome<T>(params.genome_len_, gene_gen)));
    }
    return population;
}
