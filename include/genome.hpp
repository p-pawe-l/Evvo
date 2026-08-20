/**
 * @file genome.hpp
 * @brief Defines the Genome<T> container, which owns a heap-allocated DNA
 *        sequence. Mutation/crossover are performed by free functions
 *        (see EvoPolicy<T>::mut_func_/cross_func_) operating on IndPtr<T>,
 *        not by Genome itself.
 */

#pragma once

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <utility>
#include <vector>

/**
 * @brief Owns a heap-allocated sequence of genes of type T.
 * @tparam T Gene type stored in the DNA sequence.
 */
template <typename T>
class Genome {
private:
    T* dna_seq_;
    int len_;

public:
    /**
     * @brief Accesses the raw DNA sequence.
     * @return Pointer to the first gene in the sequence.
     */
    T* data() {
        return this->dna_seq_;
    }

    /**
     * @brief Accesses the raw DNA sequence.
     * @return Pointer to the first gene in the sequence.
     */
    const T* data() const {
        return this->dna_seq_;
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
    Genome(std::initializer_list<T> seq) {
        this->dna_seq_ = new T[seq.size()];
        if (this->dna_seq_ == nullptr) {
            delete[] this->dna_seq_;
            // error while alllocating mem on the heap for genome
        }
        this->len_ = seq.size();
        for (int i = 0; i < this->len_; ++i) {
            this->dna_seq_[i] = std::move(seq.begin()[i]);
        }
    }

    /**
     * @brief Move-constructs a Genome, stealing other's DNA sequence.
     * @param other Genome to move from; left empty (nullptr, len 0).
     */
    Genome(Genome&& other) noexcept: dna_seq_{other.dna_seq_}, len_{other.len_} {
        other.dna_seq_ = nullptr;
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
            delete[] this->dna_seq_;
            this->dna_seq_ = other.dna_seq_;
            this->len_ = other.len_;
            other.dna_seq_ = nullptr;
            other.len_ = 0;
        }

        return *this;
    }

    /**
     * @brief Copy-constructs a Genome, deep-copying other's DNA sequence.
     * @param other Genome to copy from.
     */
    Genome(const Genome& other): dna_seq_{new T[other.len_]}, len_{other.len_} {
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
            T* new_seq = new T[other.len_];
            for (int i = 0; i < other.len_; ++i) {
                new_seq[i] = other.dna_seq_[i];
            }
            delete[] this->dna_seq_;
            this->dna_seq_ = new_seq;
            this->len_ = other.len_;
        }

        return *this;
    }

    /**
     * @brief Releases the DNA sequence's heap allocation, if any.
     */
    ~Genome() {
        if (this->dna_seq_ != nullptr) {
            delete[] this->dna_seq_;
        }
    }

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
