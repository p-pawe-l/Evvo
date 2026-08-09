/**
 * @file genome.hpp
 * @brief Defines the Genome<T> container, which owns a heap-allocated DNA
 *        sequence and exposes mutation/crossover operations driven by
 *        caller-supplied function pointers.
 */

#pragma once

#include <cstdint>
#include <initializer_list>
#include <utility>
#include <random>

namespace {
    /**
     * @brief Generates a uniformly distributed random value in [0, 255].
     * @return A random uint8_t.
     */
    uint8_t random_uint8_t() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<unsigned> dist(0, 255);
        return static_cast<uint8_t>(dist(gen));
    }
}

/**
 * @brief Owns a heap-allocated sequence of genes of type T and provides
 *        mutation and crossover operations over that sequence.
 * @tparam T Gene type stored in the DNA sequence.
 */
template <typename T>
class Genome {
private:
    T* dna_seq_;
    int len_;

public:
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
            this->dna_seq_[i] = std::move(seq[i]);
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

    /**
     * @brief Randomly decides whether to mutate this genome and, if so,
     *        applies the caller-supplied mutation function to its DNA.
     * @param mut_prob Probability threshold (0-255) compared against a
     *                 random draw to decide whether mutation occurs.
     * @param mut_func Function applied to the DNA sequence to perform the
     *                 mutation; returns 0 on failure, 1 on success.
     * @return 0 on mutation failure, 1 on success, 0x02 if no mutation was
     *         performed.
     */
    uint8_t mutate(uint8_t mut_prob, uint8_t (*mut_func)(T*)) {
        if (mut_prob <= random_uint8_t()) {
            uint8_t res = mut_func(this->dna_seq_);
            return res;
        }
        return 0x02;
    }

    /**
     * @brief Randomly decides whether to cross this genome with another
     *        and, if so, applies the caller-supplied crossover function.
     * @param other Genome to cross with.
     * @param cross_prob Probability threshold (0-255) compared against a
     *                    random draw to decide whether crossover occurs.
     * @param cross_func Function applied to both DNA sequences to perform
     *                   the crossover; returns 0 on failure, 1 on success.
     * @return 0 if other is null or crossover failed, 1 on success, 0x02
     *         if no crossover was performed.
     */
    uint8_t crossover(Genome<T>* other, uint8_t cross_prob, uint8_t (*cross_func)(T*, T*)) {
        if (other == nullptr) {
            return 0;
        }

        if (cross_prob <= random_uint8_t()) {
            uint8_t res = cross_func(this->dna_seq_, other->dna_seq_);
            return res;
        }
        return 0x02;
    }

};
