/**
 * @file evo_callbacks.hpp
 * @brief Declares concrete EvoCallback<double> implementations.
 *        Implemented in src/print_callback.cpp and
 *        src/save_to_file_callback.cpp.
 */

#pragma once

#include <cstdint>
#include <string>

#include "evo_callback.hpp"
#include "generation_stats.hpp"

/**
 * @brief Bitmask selecting which fields PrintCallback prints each
 *        generation. Combine with operator| (e.g.
 *        PrintField::Generation | PrintField::BestFitness).
 */
enum class PrintField : uint8_t {
    None        = 0,
    Generation  = 1u << 0,
    BestFitness = 1u << 1,
    AvgFitness  = 1u << 2,
    Genome      = 1u << 3,
    All         = Generation | BestFitness | AvgFitness | Genome,
};

/**
 * @brief Combines two PrintField flags into one bitmask.
 */
constexpr PrintField operator|(PrintField lhs, PrintField rhs) {
    return static_cast<PrintField>(
        static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

/**
 * @brief Intersects two PrintField flags.
 */
constexpr PrintField operator&(PrintField lhs, PrintField rhs) {
    return static_cast<PrintField>(
        static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

/**
 * @brief Checks whether flag is set within flags.
 * @param flags Bitmask to test.
 * @param flag Single flag to check for.
 * @return true if flag is present in flags.
 */
constexpr bool has_flag(PrintField flags, PrintField flag) {
    return (flags & flag) != PrintField::None;
}

/**
 * @brief Prints a caller-chosen subset of each generation's stats
 *        (generation number, best/average fitness, best genome's genes)
 *        to stdout.
 */
class PrintCallback : public EvoCallback<double> {
private:
    PrintField fields_;

public:
    /**
     * @brief Constructs a PrintCallback that prints the given fields.
     * @param fields Fields to print each generation, OR'd together;
     *               defaults to PrintField::All.
     */
    explicit PrintCallback(PrintField fields = PrintField::All);

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

