#pragma once

#include <vector>

#include "genome.hpp"

template <typename T>
class EvoPolicy {
public:
    virtual ~EvoPolicy() = default;

    virtual std::vector<Genome<T>*> create_new_population(
        std::vector<Genome<T>*> population,
        double (*eval_func)(Genome<T>*)) = 0;
};
