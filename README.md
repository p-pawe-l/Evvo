# areo-evo

Evolution optimizes aerodynamics of jet planes.

The idea is to represent a jet shape as a genome, evolve a population of
shapes across generations, and score each shape by feeding it through an
aerodynamics evaluation function (e.g. lift/drag from a panel method or CFD
run). Over many generations, selection, crossover, and mutation push the
population toward better-performing shapes.

The genetic-algorithm core is generic (`Genome<T>`, `Evolver<T>`, ...), so
it isn't tied to jet shapes — see [Example](#example-fitting-a-quadratic)
below for a minimal, self-contained use of the library on plain numbers.

## Architecture

The genetic-algorithm core lives in `include/` and is generic over the gene
type `T` (e.g. a set of shape control points, or a handful of `double`s).

- **`genome.hpp` — `Genome<T>`, `IndPtr<T>`, `PopulationVec<T>`**
  `Genome<T>` owns a heap-allocated (`GenomeList<T>` = `std::unique_ptr<T[]>`)
  sequence of genes. It has no crossover/mutation logic of its own — those
  live in free functions (see below) so callers can plug in whatever makes
  sense for their gene type. `IndPtr<T>` is an owning pointer to a single
  individual (`std::unique_ptr<Genome<T>>`); `PopulationVec<T>` is an owned
  population (`std::vector<IndPtr<T>>`).

- **`mut_func.hpp` — mutation functions**
  Free functions of the form `IndPtr<T> mutate_x(const IndPtr<T>& parent)`
  that return a new, mutated offspring. Includes `mutate_gaussian`,
  `mutate_random_reset`, `mutate_boundary` as ready-to-use examples.

- **`cross_func.hpp` — crossover functions**
  Free functions of the form
  `IndPtr<T> crossover_x(const IndPtr<T>& parent1, const IndPtr<T>& parent2)`
  that return a new offspring combining both parents. Includes
  `crossover_single_point`, `crossover_uniform`, `crossover_arithmetic`.

- **`population_init.hpp` — `make_random_population<T>()`**
  Builds a `PopulationVec<T>` of `population_size` genomes, each with
  `genome_len` genes, produced by calling a caller-supplied generator once
  per gene. `EvoPolicy<T>::set_random_init()` uses this internally, so in
  the common case you never need to call it directly.

- **`evo_policy.hpp` — `EvoPolicy<T>`, `PopulationEval<T>`**
  Abstract strategy interface for evolving a population: selection,
  crossover, and mutation are all applied by the policy implementation.
  Holds the fitness function (`set_eval()`), the crossover/mutation
  functions and probabilities (`set_cross_func()`, `set_mut_func()`), and
  the initial population, and exposes:
  - `set_init_population(population)` / `set_random_init(population_size,
    genome_len, lo, hi)` — configure the population `create_init_population()`
    will hand back; the latter draws each gene uniformly from `[lo, hi]`,
    picking an integer or real distribution based on `T` (see
    `random_value` in `rand_util.hpp`).
  - `create_init_population()` — hands back the population configured
    above; `Evolver<T>::run()` calls this once at the start of a run.
  - `evaluate(population)` — scores every individual exactly once, returning
    a `PopulationEval<T>` with each individual's fitness, the fittest
    individual's index/fitness, and the population's total fitness, so
    later steps (e.g. parent selection) can reuse it instead of
    re-evaluating.
  - `create_new_population(population, eval)` — produces the next
    generation, given that `PopulationEval<T>`.

- **`std_policy.hpp` / `src/std_policy.cpp` — `StdPolicy`**
  The default `EvoPolicy<double>`: selects parents via fitness-proportionate
  (roulette wheel) selection over the cached fitnesses from `evaluate()`,
  then applies the configured crossover/mutation functions.

- **`generation_stats.hpp` — `GenerationStats<T>`**
  A snapshot of one generation: generation index, population, best/average
  fitness, and the best genome found that generation.

- **`callback.hpp` / `evo_callback.hpp` — `Callback<T>` / `EvoCallback<T>`**
  `Callback<T>` is a generic interface invoked with an object of type `T`.
  `EvoCallback<T>` specializes it to `GenerationStats<T>`, so it's invoked
  once per generation with that generation's stats.

- **`evo_callbacks.hpp` / `src/*_callback.cpp` — `PrintCallback`, `SaveToFileCallback`**
  Ready-to-use `EvoCallback<double>` implementations. `PrintCallback` prints
  a caller-chosen subset of each generation's stats to stdout, selected via
  the `PrintField` bitmask (`PrintField::Generation | PrintField::BestFitness`,
  etc.; defaults to `PrintField::All`). `SaveToFileCallback` appends each
  generation's stats as a CSV row to a file (truncated on construction).

- **`evolver.hpp` — `Evolver<T>`**
  Drives the evolution loop. Given crossover/mutation functions, an
  `EvoPolicy<T>` (already configured with `set_init_population()` or
  `set_random_init()`), and a `std::vector<EvoCallback<T>*>`,
  `run(runs, eval_func)` fetches the initial population from the policy,
  then repeatedly evaluates the population, reports `GenerationStats<T>`
  to every callback (in order), and asks the policy for the next
  generation — returning the best genome found across the whole run.

## Example: fitting a quadratic

A minimal use of the library: evolve a 3-gene `Genome<double>` `{a, b, c}`
until `a*x^2 + b*x + c` fits the target function `5x^2 + 3x + 7`. Fitness is
`1 / (1 + mean squared error)` over a handful of sample points — always
positive, which `StdPolicy`'s roulette-wheel selection requires.

```cpp
#include <cstdio>

#include "cross_func.hpp"
#include "evo_callbacks.hpp"
#include "evolver.hpp"
#include "genome.hpp"
#include "mut_func.hpp"
#include "std_policy.hpp"

double target(double x) {
    return 5.0 * x * x + 3.0 * x + 7.0;
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

    Evolver<double> evolver(
        crossover_arithmetic<double>, // averages parents' genes
        mutate_gaussian<double>,      // perturbs genes with Gaussian noise
        {&print_cb},                 // callbacks, invoked in order each generation
        &policy,
        /* mutation_prob  = */ 40,   // out of 255, ~16% per offspring
        /* crossover_prob = */ 200); // out of 255, ~78% per offspring

    Genome<double> best = evolver.run(200, eval_quadratic_fit);

    std::printf("best fit: %.4fx^2 + %.4fx + %.4f\n",
        best.data()[0], best.data()[1], best.data()[2]);
}
```

The policy owns the initial population: `set_random_init()` draws
`genome_len` genes per individual uniformly from `[lo, hi]`, choosing an
integer or real distribution based on `T` — so no manual
population-building code is needed. For non-uniform or otherwise custom
initialization, build a `PopulationVec<T>` yourself (see
`make_random_population` in `population_init.hpp`, or build it by hand)
and pass it to `policy.set_init_population()` instead.

The full runnable version of this example lives in `src/main.cpp` and
builds as the `areo-evo-demo` target:

```sh
cmake -S . -B build -DAREO_BUILD_TESTS=OFF
cmake --build build --target areo-evo-demo
./build/areo-evo-demo
```

## Status

Core GA scaffolding (`Genome`, `Evolver`, `EvoPolicy`, `StdPolicy`,
mutation/crossover functions, callbacks, `GenerationStats`) is in place and
demonstrated end-to-end by the quadratic-fit example above. Still to come: a
jet-shape genome representation and the aerodynamics evaluation function
that connects a genome to a real lift/drag-based fitness score.
