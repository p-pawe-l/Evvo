// Declares concrete Callback<double> implementations. Implemented in
// src/print_callback.cpp, src/save_to_file_callback.cpp,
// src/checkpoint_callback.cpp, src/convergence_callback.cpp,
// src/fitness_threshold_callback.cpp, src/diversity_callback.cpp,
// src/best_genome_history_callback.cpp and src/stats_summary_callback.cpp.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "callback.hpp"
#include "../core/generation_stats.hpp"

// Bitmask selecting which fields PrintCallback prints each generation.
// Combine with operator| (e.g. PrintField::Generation | PrintField::BestFitness).
enum class PrintField : uint8_t {
    None = 0,
    Generation = 1U << 0,
    BestFitness = 1U << 1,
    AvgFitness = 1U << 2,
    Genome = 1U << 3,
    All = Generation | BestFitness | AvgFitness | Genome,
};

constexpr PrintField operator|(PrintField lhs, PrintField rhs) {
    return static_cast<PrintField>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

constexpr PrintField operator&(PrintField lhs, PrintField rhs) {
    return static_cast<PrintField>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

constexpr bool has_flag(PrintField flags, PrintField flag) {
    return (flags & flag) != PrintField::None;
}

class PrintCallback : public Callback<double> {
private:
    PrintField fields_;

public:
    explicit PrintCallback(PrintField fields = PrintField::All);

    void call(const GenerationStats<double>& stats) override;
};

// Truncates filename when constructed.
class SaveToFileCallback : public Callback<double> {
private:
    std::string filename_;

public:
    explicit SaveToFileCallback(std::string filename);

    void call(const GenerationStats<double>& stats) override;
};

// Overwrites filename with the current best genome every interval
// generations, so a long run can resume from the latest checkpoint after a
// crash instead of restarting from scratch.
class CheckpointCallback : public Callback<double> {
private:
    std::string filename_;
    int interval_;

public:
    explicit CheckpointCallback(std::string filename, int interval = 1);

    void call(const GenerationStats<double>& stats) override;
};

// Requests a stop once best_fitness has gone patience generations without
// improving by at least min_delta, so runs don't keep burning generations
// after plateauing.
class ConvergenceCallback : public Callback<double> {
private:
    int patience_;
    double min_delta_;
    double best_seen_ = 0.0;
    int stale_generations_ = 0;
    bool has_seen_ = false;

public:
    explicit ConvergenceCallback(int patience, double min_delta = 0.0);

    void call(const GenerationStats<double>& stats) override;
    [[nodiscard]] bool should_stop() const override;
};

// Requests a stop as soon as best_fitness reaches threshold, useful when
// the objective's goal value is known ahead of time.
class FitnessThresholdCallback : public Callback<double> {
private:
    double threshold_;
    bool reached_ = false;

public:
    explicit FitnessThresholdCallback(double threshold);

    void call(const GenerationStats<double>& stats) override;
    [[nodiscard]] bool should_stop() const override;
};

// Tracks population genetic diversity each generation, computed as the
// sum, over every gene position, of that position's variance across the
// population. A shrinking value signals the population is converging (or
// prematurely converging) around similar genomes.
class DiversityCallback : public Callback<double> {
private:
    bool print_;
    std::vector<double> history_;

public:
    explicit DiversityCallback(bool print = true);

    void call(const GenerationStats<double>& stats) override;

    [[nodiscard]] const std::vector<double>& history() const;
};

// Records a copy of the best genome found in every generation, so the
// full optimization trajectory can be inspected or plotted after the run
// finishes rather than only the final result.
class BestGenomeHistoryCallback : public Callback<double> {
public:
    struct Entry {
        int generation;
        double best_fitness;
        Genome<double> genome;
    };

private:
    std::vector<Entry> history_;

public:
    void call(const GenerationStats<double>& stats) override;

    [[nodiscard]] const std::vector<Entry>& history() const;
};

// Truncates filename when constructed.
class StatsSummaryCallback : public Callback<double> {
private:
    std::string filename_;

public:
    explicit StatsSummaryCallback(std::string filename);

    void call(const GenerationStats<double>& stats) override;
};
