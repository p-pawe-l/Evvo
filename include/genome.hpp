/**
 * @file genome.hpp
 * @brief Defines the Genome<T> container, which owns a heap-allocated DNA
 *        sequence. Mutation/crossover are performed by free functions
 *        (see EvoPolicy<T>::mut_func_/cross_func_) operating on IndPtr<T>,
 *        not by Genome itself.
 */

#pragma once

#include <initializer_list>
#include <memory>
#include <utility>
#include <vector>

/**
 * @brief Owning pointer to a heap-allocated array of genes of type T.
 * @tparam T Gene type stored in the array.
 */
template <typename T>
using GenomeList = std::unique_ptr<T[]>;

/**
 * @brief Owns a heap-allocated sequence of genes of type T.
 * @tparam T Gene type stored in the DNA sequence.
 */
template <typename T>
class Genome {
private:
    GenomeList<T> dna_seq_;
    int len_;

public:
    /**
     * @brief Accesses the raw DNA sequence.
     * @return Pointer to the first gene in the sequence.
     */
    T* data() {
        return this->dna_seq_.get();
    }

    /**
     * @brief Accesses the raw DNA sequence.
     * @return Pointer to the first gene in the sequence.
     */
    const T* data() const {
        return this->dna_seq_.get();
    }

    /**
     * @brief Number of genes in the DNA sequence.
     * @return Length of the DNA sequence.
     */
    int size() const {
        return this->len_;
    }

    /**
     * @brief Constructs a Genome from an initializer list of genes.
     * @param seq Genes to move into the newly allocated DNA sequence.
     */
    Genome(std::initializer_list<T> seq):
    dna_seq_{std::make_unique<T[]>(seq.size())},
    len_{static_cast<int>(seq.size())}
    {
        for (int i = 0; i < this->len_; ++i) {
            this->dna_seq_[i] = std::move(seq.begin()[i]);
        }
    }

    /**
     * @brief Move-constructs a Genome, stealing other's DNA sequence.
     * @param other Genome to move from; left empty (nullptr, len 0).
     */
    Genome(Genome&& other) noexcept:
    dna_seq_{std::move(other.dna_seq_)},
    len_{other.len_}
    {
        other.len_ = 0;
    }

    /**
     * @brief Move-assigns a Genome, releasing this genome's current DNA
     *        sequence and stealing other's.
     * @param other Genome to move from; left empty (nullptr, len 0).
     * @return Reference to this Genome.
     */
    Genome& operator=(Genome&& other) noexcept {
        if (this != &other) {
            this->dna_seq_ = std::move(other.dna_seq_);
            this->len_ = other.len_;
            other.len_ = 0;
        }

        return *this;
    }

    /**
     * @brief Copy-constructs a Genome, deep-copying other's DNA sequence.
     * @param other Genome to copy from.
     */
    Genome(const Genome& other):
    dna_seq_{std::make_unique<T[]>(other.len_)},
    len_{other.len_}
    {
        for (int i = 0; i < this->len_; ++i) {
            this->dna_seq_[i] = other.dna_seq_[i];
        }
    }

    /**
     * @brief Copy-assigns a Genome, deep-copying other's DNA sequence into
     *        a freshly allocated buffer before releasing the old one.
     * @param other Genome to copy from.
     * @return Reference to this Genome.
     */
    Genome& operator=(const Genome& other) {
        if (this != &other) {
            GenomeList<T> new_seq = std::make_unique<T[]>(other.len_);
            for (int i = 0; i < other.len_; ++i) {
                new_seq[i] = other.dna_seq_[i];
            }
            this->dna_seq_ = std::move(new_seq);
            this->len_ = other.len_;
        }

        return *this;
    }

    /**
     * @brief Releases the DNA sequence's heap allocation, if any.
     */
    ~Genome() = default;

};

/**
 * @brief Owning pointer to a single individual in a population.
 * @tparam T Gene type of the genome being owned.
 */
template <typename T>
using IndPtr = std::unique_ptr<Genome<T>>;

/**
 * @brief An owned population of individuals.
 * @tparam T Gene type of the genomes in the population.
 */
template <typename T>
using PopulationVec = std::vector<IndPtr<T>>;
