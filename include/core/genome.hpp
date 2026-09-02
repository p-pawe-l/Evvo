#pragma once

#include <cstddef>
#include <vector>

// Plain std::vector<T> under the alias — deep copy, move, initializer-list
// construction, .data()/.size() are already exactly what's needed, so
// there's nothing for a wrapper class to add.
template <typename T> using Genome = std::vector<T>;

// Each individual is a Genome<T> stored by value, moved rather than
// heap-allocated per individual (see docs/adr/0001-genome-by-value.md).
template <typename T> using PopulationVec = std::vector<Genome<T>>;

template <typename T, typename Generator>
Genome<T> make_genome(const std::size_t len, Generator gene_gen) {
    Genome<T> genome(len);
    for (std::size_t i = 0; i < len; ++i) {
        genome[i] = gene_gen();
    }
    return genome;
}

template <typename T> Genome<T> make_zero_genome(std::size_t len) {
    return Genome<T>(len, T{});
}

template <typename T> Genome<T> make_filled_genome(std::size_t len, const T& value) {
    return Genome<T>(len, value);
}
