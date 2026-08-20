/**
 * @file std_policy.cpp
 * @brief Defines StdPolicy, the default EvoPolicy<double> implementation
 *        used to drive standard genetic-algorithm runs over double genomes.
 */

#include "../include/evo_policy.hpp"
#include "genome.hpp"


/**
 * @brief Default EvoPolicy<double> implementation: selects parents,
 *        applies crossover/mutation, and reports fitness over a
 *        population of Genome<double>.
 */
class StdPolicy : public EvoPolicy<double> {
private:
    /**
     * @brief Selects two parent individuals from a population to breed.
     * @param population Population to select parents from.
     * @return Non-owning pointers to the two chosen parents; remain valid
     *         only as long as population is alive.
     */
    std::pair<Genome<double>*, Genome<double>*> choose_parents(
        const PopulationVec<double>& population
    )
    {
        return {nullptr, nullptr};
    }

public:
    StdPolicy() = default;
    ~StdPolicy() = default;


    /**
     * @brief Produces the next generation by repeatedly selecting parents
     *        and applying the configured crossover/mutation functions.
     * @param prev Current generation of genomes.
     * @return The new, owned population for the next generation.
     */
    PopulationVec<double> create_new_population(const PopulationVec<double>& prev) override
    {
        PopulationVec<double> new_pop;
        new_pop.reserve(prev.size());
        while (new_pop.size() != prev.size()) {
            auto parents = choose_parents(prev);
            auto offspring = std::make_unique<Genome<double>>(*parents.first);
            offspring->crossover(parents.second, crossover_prob_, cross_func_);
            offspring->mutate(mutation_prob_, mut_func_);
            new_pop.push_back(std::move(offspring));
        }
        return new_pop;
    }

    /**
     * @brief Selects the fittest genome in a population using eval_func_.
     * @param population Population to select from.
     * @return Non-owning pointer to the genome with the highest fitness;
     *         remains valid only as long as population is alive.
     */
    Genome<double>* choose_best(const PopulationVec<double>& population) override
    {
        Genome<double>* best = nullptr;
        double best_fitness = 0.0;
        for (auto& ele : population) {
            double eval = eval_func_(ele.get());
            if (best == nullptr || eval > best_fitness) {
                best_fitness = eval;
                best = ele.get();
            }
        }
        return best;
    }

    /**
     * @brief Sums fitness over a population using eval_func_.
     * @param population Population to sum fitness over.
     * @return The total fitness of the population.
     */
    double get_total_fitness(const PopulationVec<double>& population) override
    {
        double total_eval = 0.0;
        for (auto& ele : population) {
            total_eval += eval_func_(ele.get());
        }
        return total_eval;
    }
};