#pragma once

#include <algorithm>
#include <vector>
#include <functional>
#include <memory>

#include "../core/eval.hpp"
#include "../core/fitness.hpp"
#include "../core/genome.hpp"

// Drives a genetic-algorithm run over Genome<T> by repeatedly delegating
// population evolution to an EvoPolicy<T>.

namespace evvo::policy {
    class EvoPolicy;
}
namespace evvo::callback {
    class EvoCallback;
}

namespace evvo::evolver {
template <typename T,
          typename EvalT = double
>
class Evolver {
    using eval_func = std::function<EvalT(evvo::genome::Genome<T>)>;
    using wrapped_policy = std::shared_ptr<evvo::policy::EvoPolicy>;
    using wrapped_cb = std::shared_ptr<evvo::callback::EvoCallback>;

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
    Evolver(evvo::policy::EvoPolicy&& policy):
    policy_{std::make_shared<evvo::policy::EvoPolicy>(std::move(policy))}
    {}

    template <evvo::fitness::tactics::concepts::FitnessTactic Tactics>
        requires std::is_default_constructible_v<Tactics>
    void evolve(eval_func evaluation, const std::size_t runs) {
        auto counter = runs;

        policy_->set_eval_func(std::move(evaluation));
        pre_evolve_callbacks_call();
        while (counter > 0) {
            // All evolving logic here

            counter--;
        }
        post_evolve_callbacks_call();
    }
};
}