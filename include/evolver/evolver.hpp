#pragma once

#include <algorithm>
#include <initializer_list>
#include <type_traits>
#include <vector>
#include <functional>
#include <memory>

#include "../core/eval.hpp"
#include "../core/fitness.hpp"
#include "../core/genome.hpp"
#include "../policy/evo_policy.hpp"
#include "../selection/selector.hpp"

// Drives a genetic-algorithm run over Genome<T> by repeatedly delegating
// population evolution to an EvoPolicy<T>.
namespace evvo::callback {
    class EvoCallback;
}

namespace evvo::evolver {
template <typename T,
          typename EvalT = double,
          evvo::fitness::tactics::concepts::FitnessTactic Tactic = evvo::fitness::tactics::higher_better
> requires std::is_default_constructible_v<Tactic>
class Evolver {
    using eval_func = std::function<EvalT(evvo::genome::Genome<T>)>;
    using wrapped_policy = std::shared_ptr<evvo::policy::AbstractEvoPolicy<T, EvalT, Tactic>>;
    using wrapped_cb = std::shared_ptr<evvo::callback::EvoCallback>;
    using ind_selector = Selector<evvo::eval::PopulationEval<Tactic>>;

private:
    wrapped_policy policy_;
    std::vector<wrapped_cb> callbacks_; 

    void pre_evolve_callbacks_call() {
        std::ranges::for_each(callbacks_, [](auto& cb_ptr){cb_ptr->pre_call();});
    }

    template <evvo::fitness::tactics::concepts::FitnessTactic Tactics>
        requires std::is_default_constructible_v<Tactics>
    void during_evolve_callbacks_call(const evvo::eval::PopulationEval<Tactics>& eval) {
        std::ranges::for_each(callbacks_, [&eval](auto& cb_ptr){cb_ptr->call(eval); });
    }

    void post_evolve_callbacks_call() {
        std::ranges::for_each(callbacks_, [](auto& cb_ptr){cb_ptr->post_call();});
    }

public:
    Evolver(
        evvo::policy::AbstractEvoPolicy<T, EvalT, Tactic>&& policy,
        Selector<evvo::eval::PopulationEval<Tactic>>&& selector,
        std::function<evvo::genome::Genome<T>(
            const evvo::genome::Genome<T>&,
            const evvo::genome::Genome<T>&
        )> crossover_func,
        std::function<evvo::genome::Genome<T>(
            const evvo::genome::Genome<T>&
        )> mutating_func,
        std::initializer_list<wrapped_cb> callbacks
    ):
    policy_{std::make_shared<evvo::policy::AbstractEvoPolicy>(std::move(policy))} 
    {
        policy_->set_selector(std::move(selector));
        policy_->check_selector();

        policy_->set_functions(std::move(crossover_func), std::move(mutating_func));
        policy_->check_functions();

        std::ranges::for_each(callbacks.begin(), callbacks.end(), [this](auto& cb){
            callbacks_.push_back(std::move(cb));
        });
    }

    // This constructor assumes that user set policy before injecting it
    // into Evolve constructor.
    // The same goes for functions 
    Evolver(
        evvo::policy::AbstractEvoPolicy<T, EvalT, Tactic>&& policy,
        std::function<evvo::genome::Genome<T>(
            const evvo::genome::Genome<T>&,
            const evvo::genome::Genome<T>&
        )> crossover_func,
        std::function<evvo::genome::Genome<T>(
            const evvo::genome::Genome<T>&
        )> mutating_func,
        std::initializer_list<wrapped_cb> callbacks
    ): 
    policy_{std::make_shared<evvo::policy::AbstractEvoPolicy>(std::move(policy))}
    {
        policy_->check_selector();
        policy_->check_functions();
    
        std::ranges::for_each(callbacks.begin(), callbacks.end(), [this](auto& cb){
            callbacks_.push_back(std::move(cb));
        });
    }

    template <evvo::fitness::tactics::concepts::FitnessTactic Tactics = evvo::fitness::tactics::higher_better>
        requires std::is_default_constructible_v<Tactics>
    evvo::eval::PopulationEval<Tactics> evolve(eval_func evaluation, const evvo::genome::Population<T>& init_pop, const std::size_t runs, evvo::genome::PopulationMeta&& population_metadata ) {
        auto counter = runs;
        evvo::genome::Population<T> population = init_pop;
        evvo::eval::PopulationEval<Tactics> eval;

        policy_->set_eval_func(std::move(evaluation));
        pre_evolve_callbacks_call();
        while (counter > 0) {
            eval = policy_->evaluate(population);
            during_evolve_callbacks_call(eval);
            population = policy_->create_new_population(eval);

            counter--;
        }
        post_evolve_callbacks_call();
        return eval;
    }
};
}