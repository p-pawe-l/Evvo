#pragma once

#include <cstddef>

// Implementations do their one-time preparation (sorting, building a
// cumulative fitness array, etc.) in build_from_eval(), so repeated
// pick() calls across a generation stay cheap.
template <typename EvalT> class Selector {
public:
    virtual ~Selector() = default;

    virtual void build_from_eval(const EvalT& eval) = 0;

    // Returns the index of the selected individual into the population
    // the selector was constructed from.
    [[nodiscard]] virtual std::size_t pick() const = 0;
};
