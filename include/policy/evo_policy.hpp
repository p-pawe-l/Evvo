#pragma once

#include <cassert>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <memory>

#include "../core/genome.hpp"
#include "../core/fitness.hpp"
#include "../core/eval.hpp"
#include "../selection/selector.hpp"

namespace evvo::policy {

struct Probabilities {
    using f32 = float;

    f32 crossover;
    f32 mutation;
};

template <typename T,
          typename EvalT = double,
          evvo::fitness::tactics::concepts::FitnessTactic Tactics = evvo::fitness::tactics::higher_better
>
    requires std::is_default_constructible_v<Tactics>
class AbstractEvoPolicy {
    using eval_func = std::function<EvalT(const evvo::genome::Genome<T>&)>;
    using crossover_func = std::function<evvo::genome::Genome<T>(
        const evvo::genome::Genome<T>&, 
        const evvo::genome::Genome<T>&
    )>;
    using mutating_func = std::function<evvo::genome::Genome<T>(
        evvo::genome::Genome<T>&&
    )>;

    using f32 = float;

    using selector = std::unique_ptr<Selector<evvo::eval::PopulationEval<Tactics>>>;
    using id_gen = evvo::genome::GenomeIdGenerator;

protected:
    eval_func evaluation_ = nullptr;
    crossover_func crossover_func_ = nullptr;
    mutating_func mutating_func_ = nullptr;
    selector selector_;
    id_gen gen_id_;

    f32 crossover_chance_ = 0.0;
    f32 mutation_chance_ = 0.0;

public:
    explicit AbstractEvoPolicy(Probabilities&& probs);
    virtual ~AbstractEvoPolicy() = default;

    void set_crossover_func(crossover_func crossover) final {
        if (crossover == nullptr) {
            throw std::invalid_argument("Provided crossover function cannot be nullptr");
        }
        crossover_func_ = std::move(crossover);
    } 

    void set_mutating_func(mutating_func mutation) final {
        if (mutation == nullptr) {
            throw std::invalid_argument("Provided mutating function cannot be nullptr");
        }
        mutating_func_ = std::move(mutation);
    }

    void set_eval_func(eval_func evaluation) final {
        if (evaluation == nullptr) {
            throw std::invalid_argument("Provided evaluation function cannot be nullptr");
        }
        evaluation_ = std::move(evaluation);
    }

    virtual evvo::eval::PopulationEval<Tactics> evaluate() const = 0;
    virtual genome::Population<T> create_new_population() const = 0;
};



}