/**
 * @file cross_func.hpp
 * @brief Example crossover functions compatible with EvoPolicy<T>'s
 *        cross_func signature (IndPtr<T>(*)(const IndPtr<T>&, const
 *        IndPtr<T>&)). Each takes two parent individuals and returns a
 *        newly allocated offspring, deciding for itself, via
 *        Genome::data()/Genome::size(), how to combine their genes.
 */

#pragma once

#include <algorithm>
#include <memory>

#include "genome.hpp"
#include "rand_util.hpp"

/**
 * @brief Combines two parents by splitting their genomes at a single
 *        random point: genes before the point come from parent1, genes
 *        from the point onward come from parent2.
 * @tparam T Gene type of the genomes being combined.
 * @param parent1 First parent, contributing the genes before the point.
 * @param parent2 Second parent, contributing the genes from the point on.
 * @return A newly allocated offspring genome.
 */
template <typename T>
IndPtr<T> crossover_single_point(const IndPtr<T>& parent1, const IndPtr<T>& parent2) {
    auto offspring = std::make_unique<Genome<T>>(*parent1);
    int len = std::min(offspring->size(), parent2->size());
    if (len == 0) {
        return offspring;
    }
    int point = static_cast<int>(random_double(0.0, len));
    for (int i = point; i < len; ++i) {
        offspring->data()[i] = parent2->data()[i];
    }
    return offspring;
}

/**
 * @brief Combines two parents by choosing each gene independently from
 *        one parent or the other with equal probability.
 * @tparam T Gene type of the genomes being combined.
 * @param parent1 First parent.
 * @param parent2 Second parent.
 * @return A newly allocated offspring genome.
 */
template <typename T>
IndPtr<T> crossover_uniform(const IndPtr<T>& parent1, const IndPtr<T>& parent2) {
    auto offspring = std::make_unique<Genome<T>>(*parent1);
    int len = std::min(offspring->size(), parent2->size());
    for (int i = 0; i < len; ++i) {
        if (random_double(0.0, 1.0) < 0.5) {
            offspring->data()[i] = parent2->data()[i];
        }
    }
    return offspring;
}

/**
 * @brief Combines two parents by averaging each pair of genes, useful for
 *        blending numeric genomes toward the midpoint of their parents.
 * @tparam T Gene type of the genomes being combined; must support
 *           addition and division by a double.
 * @param parent1 First parent.
 * @param parent2 Second parent.
 * @return A newly allocated offspring genome.
 */
template <typename T>
IndPtr<T> crossover_arithmetic(const IndPtr<T>& parent1, const IndPtr<T>& parent2) {
    auto offspring = std::make_unique<Genome<T>>(*parent1);
    int len = std::min(offspring->size(), parent2->size());
    for (int i = 0; i < len; ++i) {
        offspring->data()[i] = static_cast<T>((offspring->data()[i] + parent2->data()[i]) / 2.0);
    }
    return offspring;
}
