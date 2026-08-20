/**
 * @file evo_callbacks.hpp
 * @brief Declares concrete EvoCallback<double> implementations.
 *        Implemented in src/print_callback.cpp, src/save_to_file_callback.cpp,
 *        src/checkpoint_callback.cpp, src/convergence_callback.cpp,
 *        src/fitness_threshold_callback.cpp, src/diversity_callback.cpp,
 *        src/best_genome_history_callback.cpp and
 *        src/stats_summary_callback.cpp.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "evo_callback.hpp"
#include "generation_stats.hpp"

/**
 * @brief Bitmask selecting which fields PrintCallback prints each
 *        generation. Combine with operator| (e.g.
 *        PrintField::Generation | PrintField::BestFitness).
 */
enum class PrintField : uint8_t {
    None = 0,
    Generation = 1U << 0,
    BestFitness = 1U << 1,
    AvgFitness = 1U << 2,
    Genome = 1U << 3,
    All = Generation | BestFitness | AvgFitness | Genome,
};

/**
 * @brief Combines two PrintField flags into one bitmask.
 */
constexpr PrintField operator|(PrintField lhs, PrintField rhs) {
    return static_cast<PrintField>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

/**
 * @brief Intersects two PrintField flags.
 */
constexpr PrintField operator&(PrintField lhs, PrintField rhs) {
    return static_cast<PrintField>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
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

/**
 * @brief Periodically overwrites a file with the current best genome, so a
 *        long run can be resumed from the latest checkpoint after a crash
 *        instead of restarting from scratch.
 */
class CheckpointCallback : public EvoCallback<double> {
private:
    std::string filename_;
    int interval_;

public:
    /**
     * @brief Constructs a CheckpointCallback that writes to filename every
     *        interval generations.
     * @param filename File overwritten with the latest checkpoint.
     * @param interval Number of generations between checkpoints; every
     *                 generation is checkpointed by default.
     */
    explicit CheckpointCallback(std::string filename, int interval = 1);

    void call(const GenerationStats<double>& stats) override;
};

/**
 * @brief Requests the evolution loop stop once best_fitness has gone
 *        patience generations without improving by at least min_delta,
 *        so runs don't keep burning generations after plateauing.
 */
class ConvergenceCallback : public EvoCallback<double> {
private:
    int patience_;
    double min_delta_;
    double best_seen_ = 0.0;
    int stale_generations_ = 0;
    bool has_seen_ = false;

public:
    /**
     * @brief Constructs a ConvergenceCallback.
     * @param patience Number of consecutive generations without a
     *                 sufficient improvement before requesting a stop.
     * @param min_delta Minimum increase in best_fitness that counts as an
     *                  improvement; defaults to 0 (any increase counts).
     */
    explicit ConvergenceCallback(int patience, double min_delta = 0.0);

    void call(const GenerationStats<double>& stats) override;
    [[nodiscard]] bool should_stop() const override;
};

/**
 * @brief Requests the evolution loop stop as soon as best_fitness reaches
 *        a caller-chosen target, useful when the objective's goal value is
 *        known ahead of time.
 */
class FitnessThresholdCallback : public EvoCallback<double> {
private:
    double threshold_;
    bool reached_ = false;

public:
    /**
     * @brief Constructs a FitnessThresholdCallback.
     * @param threshold Fitness value that, once reached or exceeded by
     *                  best_fitness, stops the run.
     */
    explicit FitnessThresholdCallback(double threshold);

    void call(const GenerationStats<double>& stats) override;
    [[nodiscard]] bool should_stop() const override;
};

/**
 * @brief Tracks population genetic diversity each generation, computed as
 *        the sum, over every gene position, of that position's variance
 *        across the population. A shrinking value signals the population
 *        is converging (or prematurely converging) around similar genomes.
 */
class DiversityCallback : public EvoCallback<double> {
private:
    bool print_;
    std::vector<double> history_;

public:
    /**
     * @brief Constructs a DiversityCallback.
     * @param print Whether to also print each generation's diversity value
     *              to stdout; defaults to true.
     */
    explicit DiversityCallback(bool print = true);

    void call(const GenerationStats<double>& stats) override;

    /**
     * @brief Diversity values recorded so far, indexed by generation.
     */
    [[nodiscard]] const std::vector<double>& history() const;
};

/**
 * @brief Records a copy of the best genome found in every generation, so
 *        the full optimization trajectory can be inspected or plotted
 *        after the run finishes rather than only the final result.
 */
class BestGenomeHistoryCallback : public EvoCallback<double> {
public:
    /**
     * @brief One recorded generation: its index, best fitness, and a deep
     *        copy of that generation's best genome.
     */
    struct Entry {
        int generation;
        double best_fitness;
        Genome<double> genome;
    };

private:
    std::vector<Entry> history_;

public:
    void call(const GenerationStats<double>& stats) override;

    /**
     * @brief Recorded entries so far, one per generation, in order.
     */
    [[nodiscard]] const std::vector<Entry>& history() const;
};

/**
 * @brief Appends each generation's fitness distribution (best, average,
 *        min, max, and standard deviation across the population) as a CSV
 *        row to a file; the file is truncated when the callback is
 *        constructed.
 */
class StatsSummaryCallback : public EvoCallback<double> {
private:
    std::string filename_;

public:
    explicit StatsSummaryCallback(std::string filename);

    void call(const GenerationStats<double>& stats) override;
};
