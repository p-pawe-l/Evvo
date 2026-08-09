/**
 * @file callback.hpp
 * @brief Defines Callback<T>, a generic interface for reacting to an
 *        object of type T (e.g. invoked once per generation by Evolver).
 */

#pragma once

/**
 * @brief Generic callback interface invoked with an object of type T.
 * @tparam T Type of the object passed to call.
 */
template <typename T>
class Callback {
public:
    virtual ~Callback() = default;

    /**
     * @brief Invoked with an object of type T.
     * @param obj Object to react to.
     */
    virtual void call(const T& obj) = 0;
};
