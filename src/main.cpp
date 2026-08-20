/**
 * @file main.cpp
 * @brief Demo: evolves a 3-gene Genome<double> {a, b, c} to fit
 *        a*x^2 + b*x + c to the target quadratic 5x^2 + 7x + 2.
 */

#include <cstdio>

#include "cross_func.hpp"
#include "evo_callbacks.hpp"
#include "evolver.hpp"
#include "genome.hpp"
#include "mut_func.hpp"
#include "std_policy.hpp"

double target(double x) {
    return (5.0 * x * x) + (7.0 * x) + 2.0;
}

double eval_quadratic_fit(Genome<double>* genome) {
    double a = genome->data()[0];
    double b = genome->data()[1];
    double c = genome->data()[2];

    double squared_error = 0.0;
    for (double x : {-5.0, -2.5, -1.0, 0.0, 1.0, 2.5, 5.0}) {
        double error = (a * x * x + b * x + c) - target(x);
        squared_error += error * error;
    }
    return 1.0 / (1.0 + squared_error / 7.0);
}

int main() {
    StdPolicy policy;
    // 100 individuals, 3 genes each (a, b, c), each drawn from [-10, 10]
    policy.set_random_init(/* population_size = */ 100, /* genome_len = */ 3, -10.0, 10.0);

    PrintCallback print_cb; // prints generation/fitness/genome each round

    Evolver<double> evolver(crossover_arithmetic<double>, // averages parents' genes
                            mutate_gaussian<double>,      // perturbs genes with Gaussian noise
                            {&print_cb}, // callbacks, invoked in order each generation
                            &policy,
                            /* mutation_prob  = */ 40,   // out of 255, ~16% per offspring
                            /* crossover_prob = */ 200); // out of 255, ~78% per offspring

    Genome<double> best = evolver.run(10000, eval_quadratic_fit);

    std::printf("best fit: %.4fx^2 + %.4fx + %.4f\n", best.data()[0], best.data()[1],
                best.data()[2]);
}
