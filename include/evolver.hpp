#pragma once

#include <cstdint>
#include <vector>

#include "genome.hpp"
#include "evo_policy.hpp"

template <typename T>
class Evolver {
private:
    std::vector<Genome<T>*> init_population_;
    uint8_t mutation_prob_;
    uint8_t crossover_prob_;
    uint8_t (*cross_func_)(T*, T*);
    uint8_t (*mut_func_)(T*);

    void (*callback_)(std::vector<Genome<T>*>);

    Genome<T>* best_genome_;
    EvoPolicy<T>* policy_;


public:
    Evolver(std::vector<Genome<T>*> init_population,
            uint8_t (*cross_func)(T*, T*),
            uint8_t (*mut_func)(T*),
            uint8_t mutation_prob,
            uint8_t crossover_prob):
    init_population_{init_population},
    mutation_prob_{mutation_prob},
    crossover_prob_{crossover_prob},
    cross_func_{cross_func},
    mut_func_{mut_func}
    {}

    ~Evolver() = default;

    /**
    * @brief Runs Evolution process and returns best Genome;
    */
    Genome<T> run(int runs, double (*eval_func)(Genome<T>*)) {
        std::vector<Genome<T>*> new_pop = this->init_population_;
        while(runs > 0) {
            this->callback_(new_pop);
            new_pop = this->policy_->create_new_population(new_pop, eval_func);
            runs--;
        }


        return *(this->best_genome_);
    }



};
