#pragma once

#include <vector>

#include "fitness.hpp"

namespace evvo::eval {

struct GenomeEntry {
    std::size_t id;
    std::size_t population_index;
    double fitness;
};

template <evvo::fitness::tactics::concepts::FitnessTactic Tactics>
    requires std::default_initializable<Tactics>
struct PopulationEval {
private:
    std::vector<GenomeEntry> entries;
    std::size_t best_index;
    double best_fitness;
    double total_fitness;

public:
    explicit PopulationEval(std::size_t pop_size):
    best_fitness{Tactics::get_worst_fit()} {
        // Reserve memory at construction time
        entries.reserve(pop_size);
    }

    void update(const GenomeEntry& data) {
        if (evvo::fitness::is_better<Tactics>(data.fitness, best_fitness)) {
            best_index = data.population_index;
            best_fitness = data.fitness;
        }
        total_fitness += data.fitness;
        entries.push_back(data);
    }       

    std::vector<GenomeEntry>::iterator begin() {
        return entries.begin();
    }

    std::vector<GenomeEntry>::iterator end() {
        return entries.end();
    }
    
};

}