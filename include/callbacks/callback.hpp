#pragma once

#include "../core/generation_stats.hpp"
#include "../core/genome.hpp"

// Hook interface invoked around and during an Evolver<T> run, subclassed
// directly by every concrete callback.
template <typename T> class Callback {
public:
    virtual ~Callback() = default;

    virtual void call(const GenerationStats<T>& stats) = 0;

    virtual void pre_run_call() {}

    virtual void post_run_call(const Genome<T>& run_champion) {}

    // Polled by Evolver<T>::evolve() right after call(); if any callback
    // returns true, the run stops before producing the next generation.
    [[nodiscard]] virtual bool should_stop() const { return false; }
};
