/**
 * @file genome.hpp
 * @brief Defines Genome<T>, an individual's DNA sequence, and the
 *        make_*_genome() functions used to build one.
 */

#pragma once

#include <cstddef>
#include <vector>

/**
 * @brief An individual's DNA sequence: a runtime-sized sequence of genes
 *        of type T. Plain std::vector<T> under the alias — deep copy,
 *        move, initializer-list construction, .data()/.size() are all
 *        already exactly what's needed, so there's nothing for a wrapper
 *        class to add.
 * @tparam T Gene type stored in the sequence.
 */
template <typename T>
using Genome = std::vector<T>;

/**
 * @brief Builds a Genome of len genes, each produced by invoking
 *        gene_gen once per gene (e.g. a random-value generator).
 * @tparam T Gene type of the genome to build.
 * @tparam Generator Callable with signature T().
 * @param len Number of genes to generate.
 * @param gene_gen Invoked once per gene to produce its value.
 * @return The newly built genome.
 */
template <typename T, typename Generator>
Genome<T> make_genome(const std::size_t len, Generator gene_gen) {
    Genome<T> genome(len);
    for (std::size_t i = 0; i < len; ++i) {
        genome[i] = gene_gen();
    }
    return genome;
}

/**
 * @brief Builds a Genome of len genes, all zero-initialized.
 * @tparam T Gene type of the genome to build; must be value-initializable.
 * @param len Number of genes.
 * @return The newly built, all-zero genome.
 */
template <typename T>
Genome<T> make_zero_genome(std::size_t len) {
    return Genome<T>(len, T{});
}

/**
 * @brief Builds a Genome of len genes, all set to the same value.
 * @tparam T Gene type of the genome to build.
 * @param len Number of genes.
 * @param value Value every gene is set to.
 * @return The newly built, uniformly-valued genome.
 */
template <typename T>
Genome<T> make_filled_genome(std::size_t len, const T& value) {
    return Genome<T>(len, value);
}
