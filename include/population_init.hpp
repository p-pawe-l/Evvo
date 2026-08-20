/**
 * @file population_init.hpp
 * @brief Helper for building an initial PopulationVec<T> without hand-
 *        rolling the same generation loop at every call site.
 */

#pragma once

#include <cstddef>

#include "genome.hpp"

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
PopulationVec<T> make_random_population(std::size_t population_size, std::size_t genome_len,
                                        Generator gene_gen) {
    PopulationVec<T> population;
    population.reserve(population_size);
    for (std::size_t i = 0; i < population_size; ++i) {
        population.push_back(std::make_unique<Genome<T>>(genome_len, gene_gen));
    }
    return population;
}
