#pragma once

#include <cstddef>

namespace evvo::selection {

template <typename EvalT> 
class AbstractSelector {
public:
    virtual ~AbstractSelector() = default;
    virtual void build_from_eval(const EvalT& eval) = 0;
    [[nodiscard]] virtual std::size_t pick() const = 0;
};

}