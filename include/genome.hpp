#pragma once

#include <cstdint>
#include <initializer_list>
#include <utility>
#include <random>

namespace {
    uint8_t random_uint8_t() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<unsigned> dist(0, 255);
        return static_cast<uint8_t>(dist(gen));
    }
}

template <typename T>
class Genome {
private:
    T* dna_seq_;
    int len_;

public:
    constexpr Genome(std::initializer_list<T> seq) {
        this->dna_seq_ = new T[sizeof(T) * seq.size()];
        if (this->dna_seq_ == nullptr) {
            delete[] this->dna_seq_;
            // error while alllocating mem on the heap for genome
        }
        this->len_ = seq.size();
        for (int i = 0; i < this->len_; ++i) {
            this->dna_seq_[i] = std::move(seq[i]);
        }
    }

    ~Genome() {
        if (this->dna_seq_ != nullptr) {
            delete[] this->dna_seq_;
        }
    }

    uint8_t mutate(uint8_t mut_prob, uint8_t (*mut_func)(T*)) {
        if (mut_prob <= random_uint8_t()) {
            uint8_t res = mut_func(this->dna_seq_);
            // 0 -> something goes wrong while mutating
            // 1 -> mutation applied sucessfully
            return res;
        }
        // No mutation performed
        return 0x02;
    }

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
