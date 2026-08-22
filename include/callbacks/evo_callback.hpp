/**
 * @file evo_callback.hpp
 * @brief Defines EvoCallback<T>, a Callback specialized to react to
 *        GenerationStats<T> produced by Evolver<T>.
 */

#pragma once

#include "callback.hpp"
#include "../core/generation_stats.hpp"

/**
 * @brief Callback specialized for GenerationStats<T>.
 * @tparam T Gene type of the genomes being evolved.
 */
template <typename T> class EvoCallback : public Callback<GenerationStats<T>> {
public:
    void call(const GenerationStats<T>& stats) override {
        // TODO: react to generation stats (logging, checkpointing, etc.)
    }

    /**
     * @brief Polled by Evolver<T>::run() right after call(); if any
     *        callback returns true, the run stops before producing the
     *        next generation. Defaults to false (never stop).
     * @return true to request the evolution loop stop.
     */
    [[nodiscard]] virtual bool should_stop() const { return false; }
};
