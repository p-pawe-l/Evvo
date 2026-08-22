/**
 * @file population_init.hpp
 * @brief Helper for building an initial PopulationVec<T> without hand-
 *        rolling the same generation loop at every call site.
 */

#pragma once

#include <cstddef>

#include "../core/genome.hpp"

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
 * @param population_size Number of individuals to generate.
 * @param genome_len Number of genes per individual.
 * @param gene_gen Invoked once per gene to produce its value.
 * @return The newly built population.
 */
template <typename T, typename Generator>
PopulationVec<T> make_random_population(const InitPopSizeParams& params, Generator gene_gen) {
    PopulationVec<T> population;
    population.reserve(params.population_size_);
    for (std::size_t i = 0; i < params.population_size_; ++i) {
        population.push_back(std::make_unique<Genome<T>>(params.genome_len_, gene_gen));
    }
    return population;
}
