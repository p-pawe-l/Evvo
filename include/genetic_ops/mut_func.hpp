/**
 * @file mut_func.hpp
 * @brief Example mutation functions compatible with EvoPolicy<T>'s mut_func
 *        signature (IndPtr<T>(*)(const IndPtr<T>&)). Each takes a parent
 *        individual and returns a newly allocated, mutated offspring,
 *        deciding for itself, via Genome::data()/Genome::size(), which
 *        genes to change and how.
 */

#pragma once

#include <memory>
#include <random>

#include "core/genome.hpp"
#include "util/rand_util.hpp"

/**
 * @brief Perturbs every gene by adding zero-mean Gaussian noise, useful
 *        for making small local adjustments around the current values.
 * @tparam T Gene type; must support += with a double.
 * @param individual Parent individual to mutate.
 * @return A newly allocated offspring with the mutation applied.
 */
template <typename T> IndPtr<T> mutate_gaussian(const IndPtr<T>& individual) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<double> noise(0.0, 0.1);
    auto offspring = std::make_unique<Genome<T>>(*individual);
    for (int i = 0; i < offspring->size(); ++i) {
        offspring->data()[i] += noise(gen);
    }
    return offspring;
}

/**
 * @brief Replaces a single, randomly chosen gene with a fresh uniformly
 *        random value in [-1, 1], useful for escaping local optima
 *        without disturbing the rest of the genome.
 * @tparam T Gene type; must be assignable from a double.
 * @param individual Parent individual to mutate.
 * @return A newly allocated offspring with the mutation applied; an
 *         unchanged copy of individual if it has no genes.
 */
template <typename T> IndPtr<T> mutate_random_reset(const IndPtr<T>& individual) {
    auto offspring = std::make_unique<Genome<T>>(*individual);
    if (offspring->size() == 0) {
        return offspring;
    }
    int i = static_cast<int>(random_double(0.0, offspring->size()));
    offspring->data()[i] = static_cast<T>(random_double(-1.0, 1.0));
    return offspring;
}

/**
 * @brief Snaps every gene to one of its boundary values (-1 or 1), useful
 *        for exploring extreme values across the whole search space.
 * @tparam T Gene type; must be assignable from a double.
 * @param individual Parent individual to mutate.
 * @return A newly allocated offspring with the mutation applied.
 */
template <typename T> IndPtr<T> mutate_boundary(const IndPtr<T>& individual) {
    auto offspring = std::make_unique<Genome<T>>(*individual);
    for (int i = 0; i < offspring->size(); ++i) {
        offspring->data()[i] = static_cast<T>((random_double(0.0, 1.0) < 0.5) ? -1.0 : 1.0);
    }
    return offspring;
}
