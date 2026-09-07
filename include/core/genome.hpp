#pragma once

#include <cstddef>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <utility>

namespace evvo::genome {
    
    template <typename T> 
    using Genome = std::vector<T>;

    template <typename genT, typename Generator> requires std::is_arithmetic_v<genT>
    Genome<genT> make_genome(const std::size_t len, Generator&& gene_gen) {
        Genome<genT> genome;
        genome.reserve(len);
        std::ranges::for_each_n(genome.begin(), len, [&gene_gen](genT&& ele) {
            ele = gene_gen();
        });
        return genome;
    }

    template <typename genT>
    using Population = std::vector<Genome<genT>>;

    struct population_meta {
        std::size_t population_size;
        std::size_t genome_size;
    };

    template <typename genT, typename Generator> requires std::is_arithmetic_v<genT>
    Population<genT> make_population(const population_meta& arg, Generator&& gene_gen) {
        Population<genT> population;
        population.reserve(arg.population_size);
        std::ranges::for_each(population.begin(), arg.population_size, [&gene_gen, &arg](Genome<genT>& ele) {
            ele = std::move(make_genome(arg.genome_size, gene_gen));
        });
        return population;
    }
}