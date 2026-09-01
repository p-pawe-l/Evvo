/**
 * @file callback.hpp
 * @brief Defines Callback<T>, the single hook interface for reacting to an
 *        Evolver<T> run over genomes of gene type T.
 */

#pragma once

#include "../core/generation_stats.hpp"
#include "../core/genome.hpp"

/**
 * @brief Hook interface invoked around and during an Evolver<T> run.
 *        Subclassed directly by every concrete callback.
 * @tparam T Gene type of the genomes being evolved.
 */
template <typename T> class Callback {
public:
    virtual ~Callback() = default;

    /**
     * @brief Invoked once per generation with that generation's stats.
     * @param stats Snapshot of the generation just evaluated.
     */
    virtual void call(const GenerationStats<T>& stats) = 0;

    /**
     * @brief Invoked once before the first generation.
     */
    virtual void pre_run_call() {}

    /**
     * @brief Invoked once after the run ends.
     * @param run_champion Best genome found across the whole run.
     */
    virtual void post_run_call(const Genome<T>& run_champion) {}

    /**
     * @brief Polled by Evolver<T>::evolve() right after call(); if any
     *        callback returns true, the run stops before producing the
     *        next generation. Defaults to false (never stop).
     * @return true to request the evolution loop stop.
     */
    [[nodiscard]] virtual bool should_stop() const { return false; }
};
