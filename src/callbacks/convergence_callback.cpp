/**
 * @file convergence_callback.cpp
 * @brief Implements ConvergenceCallback.
 */

#include "callbacks/evo_callbacks.hpp"

ConvergenceCallback::ConvergenceCallback(int patience, double min_delta)
    : patience_{patience}, min_delta_{min_delta} {}

void ConvergenceCallback::call(const GenerationStats<double>& stats) {
    if (!this->has_seen_ || stats.best_fitness > this->best_seen_ + this->min_delta_) {
        this->best_seen_ = stats.best_fitness;
        this->has_seen_ = true;
        this->stale_generations_ = 0;
    } else {
        ++this->stale_generations_;
    }
}

bool ConvergenceCallback::should_stop() const {
    return this->stale_generations_ >= this->patience_;
}
