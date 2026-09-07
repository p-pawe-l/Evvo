#pragma once

#include <cstddef>
#include <type_traits>
#include <vector>

namespace evvo::genome {
    
    template <typename T> 
    using Genome = std::vector<T>;

    template <typename T, typename Generator> requires std::is_arithmetic_v<T>
    Genome<T> make_genome(const std::size_t len, Generator&& gene_gen) {
        Genome<T> genome;
        genome.reserve(len);
        for (std::size_t i = 0; i < len; ++i) { genome.push_back(gene_gen()); }
        return genome;
    }

    template <typename T>
    using Population = std::vector<Genome<T>>;

    struct PopulationMeta {
        std::size_t population_size;
        std::size_t genome_size;
    };

    template <typename T, typename Generator> requires std::is_arithmetic_v<T>
    Population<T> make_population(const PopulationMeta& arg, Generator&& gene_gen) {
        Population<T> population;
        population.reserve(arg.population_size);
        for (std::size_t i = 0; i < arg.population_size; ++i) {
            population.push_back(make_genome<T>(arg.genome_size, gene_gen));
        }
        return population;
    }
}