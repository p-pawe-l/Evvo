/**
 * @file selector.hpp
 * @brief Defines Selector, the interface every parent-selection strategy
 *        implements.
 */

#pragma once

#include <cstddef>

/**
 * @brief Interface for parent-selection strategies. Implementations do
 *        their one-time preparation (sorting, building a cumulative
 *        fitness array, etc.) in their constructor, so repeated pick()
 *        calls across a generation stay cheap.
 */
class Selector {
public:
    virtual ~Selector() = default;

    /**
     * @brief Selects a single individual.
     * @return Index of the selected individual into the population the
     *         selector was constructed from.
     */
    [[nodiscard]] virtual std::size_t pick() const = 0;
};
