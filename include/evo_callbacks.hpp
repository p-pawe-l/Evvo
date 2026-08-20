/**
 * @file evo_callbacks.hpp
 * @brief Declares concrete EvoCallback<double> implementations.
 *        Implemented in src/print_callback.cpp and
 *        src/save_to_file_callback.cpp.
 */

#pragma once

#include <string>

#include "evo_callback.hpp"
#include "generation_stats.hpp"

/**
 * @brief Prints each generation's best/average fitness and the best
 *        genome's genes to stdout.
 */
class PrintCallback : public EvoCallback<double> {
public:
    void call(const GenerationStats<double>& stats) override;
};

/**
 * @brief Appends each generation's best/average fitness and the best
 *        genome's genes as a CSV row to a file; the file is truncated
 *        when the callback is constructed.
 */
class SaveToFileCallback : public EvoCallback<double> {
private:
    std::string filename_;

public:
    explicit SaveToFileCallback(std::string filename);

    void call(const GenerationStats<double>& stats) override;
};

