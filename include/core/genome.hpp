#pragma once

#include <cstddef>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <optional>
#include <array>

namespace evvo::genome {
    
    // Plain std::vector<T> under the alias — deep copy, move, initializer-list
    // construction, .data()/.size() are already exactly what's needed, so
    // there's nothing for a wrapper class to add.
    template <typename T> 
    using Genome = std::vector<T>;
    
    template <typename T, std::size_t N>
    using FixedGenome = std::array<T, N>;

    // Each individual is a Genome<T> stored by value, moved rather than
    // heap-allocated per individual (see docs/adr/0001-genome-by-value.md).
    template <typename T> 
    using PopulationVec = std::vector<Genome<T>>;
    
    template <typename T, typename Generator> requires std::is_arithmetic_v<T>
    Genome<T> make_genome(const std::size_t len, Generator&& gene_gen) {
        Genome<T> genome;
        genome.reserve(len);
        for (std::size_t i = 0; i < len; ++i) {
            genome[i] = gene_gen();
        }
        return genome;
    }

    template <typename T, std::size_t N, typename Generator> requires std::is_arithmetic_v<T>
    FixedGenome<T, N> make_fixed_genome(Generator&& gene_gen) {
        FixedGenome<T, N> genome;
        for (std::size_t i = 0; i < N; ++i) {
            genome[i] = gene_gen();
        }
        return genome;
    }
}

namespace {

    template <typename GenomeType,
              typename HashStorage = std::unordered_map<std::size_t, evvo::genome::Genome<GenomeType>>
    >
    class genome_registry {
        using storage = HashStorage;
        using storage_thread_lock = std::lock_guard<std::mutex>;

    private:
        storage registry_;
        std::mutex registry_lock_;
 
    public:

        void put();


    };
}

namespace evvo::genome::registry {

    std::size_t get_new_id();

    template <typename GenomeType> requires std::is_arithmetic_v<GenomeType>
    std::optional<std::size_t> get_id(const Genome<GenomeType>& ind) {
            
    } 
}