#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

#include "core/genome.hpp"
#include "core/evo_policy.hpp"
#include "callbacks/callback.hpp"
#include "core/generation_stats.hpp"
#include "selection/selector.hpp"

// Drives a genetic-algorithm run over Genome<T> by repeatedly delegating
// population evolution to an EvoPolicy<T>.
template <typename T> class Evolver {
private:
    std::vector<Callback<T>*> callbacks_;
    EvoPolicy<T>* policy_;
    std::unique_ptr<Selector<PopulationEval>> selector_;

    void notify_pre_run() {
        for (Callback<T>* callback : this->callbacks_) {
            callback->pre_run_call();
        }
    }

    void notify_post_run(const Genome<T>& run_champion) {
        for (Callback<T>* callback : this->callbacks_) {
            callback->post_run_call(run_champion);
        }
    }

    // Reports stats to every callback; returns true if any requests a stop.
    bool notify_generation(const GenerationStats<T>& stats) {
        bool stop = false;
        for (Callback<T>* callback : this->callbacks_) {
            callback->call(stats);
            stop = stop || callback->should_stop();
        }
        return stop;
    }

    static void update_run_champion(std::unique_ptr<Genome<T>>& champion, double& champion_fitness,
                                    const Genome<T>& candidate, double candidate_fitness) {
        if (champion == nullptr || candidate_fitness > champion_fitness) {
            champion = std::make_unique<Genome<T>>(candidate);
            champion_fitness = candidate_fitness;
        }
    }

public:
    // selector is owned by this Evolver for its whole lifetime and handed
    // to policy via EvoPolicy<T>::set_selector(). policy must have
    // set_init_population()/set_random_init() called on it before evolve().
    template <typename SelectorT>
    Evolver(std::function<Genome<T>(const Genome<T>&, const Genome<T>&)> cross_func,
            std::function<Genome<T>(const Genome<T>&)> mut_func,
            std::vector<Callback<T>*> callbacks, EvoPolicy<T>* policy, SelectorT selector,
            uint8_t mutation_prob, uint8_t crossover_prob)
        : callbacks_{std::move(callbacks)}, policy_{policy},
          selector_{std::make_unique<SelectorT>(std::move(selector))} {
        static_assert(std::is_base_of_v<Selector<PopulationEval>, SelectorT>,
                      "Evolver: selector must derive from Selector<PopulationEval>");
        this->policy_->set_cross_func(std::move(cross_func), crossover_prob);
        this->policy_->set_mut_func(std::move(mut_func), mutation_prob);
        this->policy_->set_selector(this->selector_.get());
    }

    ~Evolver() = default;

    // eval_func must not mutate the genome it's given. Returns the Run
    // Champion: the best genome found across the whole run.
    Genome<T> evolve(int runs, std::function<double(const Genome<T>*)> eval_func) {
        this->policy_->set_eval(std::move(eval_func));
        this->notify_pre_run();

        PopulationVec<T> new_pop = this->policy_->create_init_population();
        int generation = 0;

        std::unique_ptr<Genome<T>> run_champion = nullptr;
        double run_champion_fitness = 0.0;

        while (runs > 0) {
            if (new_pop.empty()) {
                break;
            }

            PopulationEval eval = this->policy_->evaluate(new_pop);
            Genome<T>* generation_champion = &new_pop[eval.best_index];
            update_run_champion(run_champion, run_champion_fitness, *generation_champion,
                                eval.best_fitness);

            GenerationStats<T> stats{generation,          new_pop,
                                     eval.best_fitness,   eval.total_fitness / new_pop.size(),
                                     generation_champion, eval.fitnesses};
            if (this->notify_generation(stats)) {
                break;
            }

            new_pop = this->policy_->create_new_population(new_pop, eval);
            ++generation;
            runs--;
        }

        this->notify_post_run(*run_champion);
        return *run_champion;
    }
};
