#pragma once

#include <algorithm>

#include "../core/genome.hpp"
#include "../util/rand_util.hpp"

// Add to each gene noise provided by gaussian distribution
// Mean of the distribution is equal to gene value,
// but standard deviation is provided by user. Higher 
// std dev means more probability of having more various individual
// than the previous one.
template <typename T, double std_dev> 
Genome<T> mutate_gaussian(const Genome<T>& individual) {
    Genome<T> offspring = individual;
    std::ranges::for_each(offspring, [](T& gene) { 
            gene += random_gaussian(gene, std_dev); 
    });
    return offspring;
}

// Just adds a random number to 'kgene; number of genes.
template <typename T, std::size_t kgenes = 1, double low, double high>
Genome<T> mutate_by_random(const Genome<T>& individual) {
    Genome<T> offspring = individual;
    if (offspring.empty()) {
        return offspring;
    }
    for (std::size_t i = 0; i < kgenes; ++i) {
        auto k = static_cast<int>(random_double(0.0, offspring.size()));
        offspring[k] += static_cast<T>(random_double(low, high));
    }
    return offspring;
}

// Replace value in genome with random one. Performed for kgenes number of genes.
template <typename T, std::size_t kgenes = 1, double lo, double hi>
Genome<T> mutate_random_reset(const Genome<T>& individual) {
    Genome<T> offspring = individual;
    if (offspring.empty()) {
        return offspring;
    }
    for (std::size_t i = 0; i < kgenes; ++i) {
        int k = static_cast<int>(random_double(0.0, offspring.size()));
        offspring[k] = static_cast<T>(random_double(lo, hi));
    }
    return offspring;
}
