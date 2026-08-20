/**
 * @file fitness_threshold_callback.cpp
 * @brief Implements FitnessThresholdCallback.
 */

#include "evo_callbacks.hpp"

FitnessThresholdCallback::FitnessThresholdCallback(double threshold) : threshold_{threshold} {}

void FitnessThresholdCallback::call(const GenerationStats<double>& stats) {
    if (stats.best_fitness >= this->threshold_) {
        this->reached_ = true;
    }
}

bool FitnessThresholdCallback::should_stop() const {
    return this->reached_;
}
