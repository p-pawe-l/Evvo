# Evvo

A small, generic genetic-algorithm library in C++20. The core
(`Genome<T>`, `Evolver<T>`, `EvoPolicy<T>`, ...) isn't tied to any
particular problem domain — it operates on genomes of any gene type `T`,
with crossover, mutation, and fitness evaluation all supplied by the
caller. See [Example](#example-fitting-a-quadratic) below for a minimal,
self-contained use of the library on plain numbers.

## Architecture

The genetic-algorithm core lives in `include/`, generic over the gene
type `T` (e.g. plain `double`s, or a custom struct of parameters).

- **`genome.hpp` — `Genome<T>`, `PopulationVec<T>`**
  `Genome<T>` is a plain `std::vector<T>` alias: a runtime-sized sequence
  of genes, deep-copied and moved like any vector. It has no
  crossover/mutation logic of its own — those live in free functions (see
  below) so callers can plug in whatever makes sense for their gene type.
  `PopulationVec<T>` is an owned population (`std::vector<Genome<T>>`) —
  each individual is a `Genome<T>` stored by value, so building or
  reproducing a population moves genomes around rather than heap-allocating
  a wrapper per individual.

- **`mut_func.hpp` — mutation functions**
  Free functions of the form `Genome<T> mutate_x(const Genome<T>& parent)`
  that return a new, mutated offspring. Includes `mutate_gaussian`,
  `mutate_random_reset`, `mutate_boundary` as ready-to-use examples.

- **`cross_func.hpp` — crossover functions**
  Free functions of the form
  `Genome<T> crossover_x(const Genome<T>& parent1, const Genome<T>& parent2)`
  that return a new offspring combining both parents. Includes
  `crossover_single_point`, `crossover_uniform`, `crossover_arithmetic`.

- **`population_init.hpp` — `generate_population<T>()`, `Sampler<T>`**
  Builds a `PopulationVec<T>` of `population_size` genomes, each with
  `genome_len` genes, produced by calling a caller-supplied `Sampler<T>`
  once per gene (`RandomSampler<T>` for a uniform range, `ValueSampler<T>`
  for a fixed value — see `core/samplers/`). `EvoPolicy<T>::set_random_init()`
  uses this internally with a `RandomSampler<T>`, so in the common case you
  never need to call it directly.

- **`evo_policy.hpp` — `EvoPolicy<T>`, `PopulationEval<T>`**
  Abstract strategy interface for evolving a population: selection,
  crossover, and mutation are all applied by the policy implementation.
  Holds the fitness function (`set_eval()`), the crossover/mutation
  functions and probabilities (`set_cross_func()`, `set_mut_func()`), and
  the initial population, and exposes:
  - `set_init_population(population)` / `set_random_init(population_size,
    genome_len, lo, hi)` — configure the population `create_init_population()`
    will hand back; the latter builds a `RandomSampler<T>` over `[lo, hi]`
    and draws each gene from it via `generate_population()`.
  - `create_init_population()` — hands back the population configured
    above; `Evolver<T>::evolve()` calls this once at the start of a run.
  - `evaluate(population)` — scores every individual exactly once, returning
    a `PopulationEval<T>` with each individual's fitness, the fittest
    individual's index/fitness, and the population's total fitness, so
    later steps (e.g. parent selection) can reuse it instead of
    re-evaluating.
  - `create_new_population(population, eval)` — produces the next
    generation, given that `PopulationEval<T>`.
  - `set_selector(selector)` — sets the `Selector<PopulationEval>` used for
    parent selection; `Evolver<T>` constructs the selector it's given and
    wires it in.

- **`selector.hpp` — `Selector<EvalT>`**
  Interface for parent-selection strategies: `build_from_eval(eval)` does
  one-time preparation for a generation (sorting, building a cumulative
  fitness array, etc.), then `pick()` cheaply selects one individual's
  index, callable repeatedly across the generation. `RankSelector`
  (`rank_selector.hpp`) and `RouletteSelector` (`roulette_selector.hpp`)
  are the two ready-to-use implementations.

- **`std_policy.hpp` / `src/std_policy.cpp` — `StdPolicy`**
  The default `EvoPolicy<double>`: selects parents via whichever
  `Selector<PopulationEval>` was configured (e.g. `RankSelector`,
  `RouletteSelector`), then applies the configured crossover/mutation
  functions.

- **`generation_stats.hpp` — `GenerationStats<T>`**
  A snapshot of one generation: generation index, population, best/average
  fitness, that generation's champion, and every individual's fitness
  (indexed the same as the population).

- **`callback.hpp` — `Callback<T>`**
  The single hook interface for reacting to an `Evolver<T>` run over
  genomes of gene type `T`, subclassed directly by every concrete callback.
  `call(const GenerationStats<T>&)` fires once per generation;
  `pre_run_call()` fires once before the first generation; `post_run_call(const
  Genome<T>&)` fires once after the run ends, with the Run Champion. It also
  exposes `should_stop()` (default `false`); if any callback returns `true`
  after a generation, `Evolver<T>::evolve()` stops early instead of running
  the remaining generations.

- **`evo_callbacks.hpp` / `src/*_callback.cpp` — ready-to-use `Callback<double>` implementations**
  - `PrintCallback` — prints a caller-chosen subset of each generation's
    stats to stdout, selected via the `PrintField` bitmask
    (`PrintField::Generation | PrintField::BestFitness`, etc.; defaults to
    `PrintField::All`).
  - `SaveToFileCallback` — appends each generation's best/average fitness
    and the best genome's genes as a CSV row to a file (truncated on
    construction).
  - `CheckpointCallback` — periodically overwrites a file with the current
    best genome, so a long run can resume from the latest checkpoint
    instead of restarting from scratch.
  - `ConvergenceCallback` — requests an early stop once `best_fitness` has
    gone a caller-chosen number of generations without improving by at
    least `min_delta`.
  - `FitnessThresholdCallback` — requests an early stop as soon as
    `best_fitness` reaches a caller-chosen target value.
  - `DiversityCallback` — tracks population genetic diversity each
    generation (sum of per-gene variance across the population), useful for
    spotting premature convergence.
  - `BestGenomeHistoryCallback` — records a copy of the best genome from
    every generation, so the full optimization trajectory can be inspected
    after the run finishes.
  - `StatsSummaryCallback` — appends each generation's fitness distribution
    (best, average, min, max, standard deviation) as a CSV row to a file.

- **`evolver.hpp` — `Evolver<T>`**
  Drives the evolution loop. Given crossover/mutation functions, an
  `EvoPolicy<T>` (already configured with `set_init_population()` or
  `set_random_init()`), and a `std::vector<Callback<T>*>`,
  `evolve(runs, eval_func)` calls `pre_run_call()` on every callback, fetches
  the initial population from the policy, then repeatedly evaluates the
  population, reports `GenerationStats<T>` to every callback (in order), and
  asks the policy for the next generation — stopping early if a callback
  requests it — before calling `post_run_call()` on every callback with the
  Run Champion and returning it.

## Example: fitting a quadratic

A minimal use of the library: evolve a 3-gene `Genome<double>` `{a, b, c}`
until `a*x^2 + b*x + c` fits the target function `5x^2 + 7x + 2`. Fitness is
`1 / (1 + mean squared error)` over a handful of sample points — always
positive, which `RouletteSelector`'s fitness-proportionate selection
requires (this example uses `RankSelector` instead, which has no such
constraint).

```cpp
#include <cstdio>

#include "cross_func.hpp"
#include "evo_callbacks.hpp"
#include "evolver.hpp"
#include "genome.hpp"
#include "mut_func.hpp"
#include "rank_selector.hpp"
#include "std_policy.hpp"

double target(double x) {
    return 5.0 * x * x + 7.0 * x + 2.0;
}

double eval_quadratic_fit(const Genome<double>* genome) {
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
        RankSelector(RankSelectionType::LINEAR, 0.7), // parent selection
        /* mutation_prob  = */ 40,   // out of 255, ~16% per offspring
        /* crossover_prob = */ 200); // out of 255, ~78% per offspring

    Genome<double> best = evolver.evolve(200, eval_quadratic_fit);

    std::printf("best fit: %.4fx^2 + %.4fx + %.4f\n",
        best.data()[0], best.data()[1], best.data()[2]);
}
```

The policy owns the initial population: `set_random_init()` draws
`genome_len` genes per individual uniformly from `[lo, hi]`, choosing an
integer or real distribution based on `T` — so no manual
population-building code is needed. For non-uniform or otherwise custom
initialization, build a `PopulationVec<T>` yourself (see
`generate_population` in `population_init.hpp`, or build it by hand)
and pass it to `policy.set_init_population()` instead.

The full runnable version of this example lives in `src/main.cpp` and
builds as the `evvo-demo` target:

```sh
cmake -S . -B build -DEVVO_BUILD_TESTS=OFF
cmake --build build --target evvo-demo
./build/evvo-demo
```

## Using Evvo from another project

Pull it in with `FetchContent`, pinned to a tag:

```cmake
include(FetchContent)
FetchContent_Declare(
    Evvo
    GIT_REPOSITORY https://github.com/p-pawe-l/Evvo.git
    GIT_TAG        v0.1.0
)
set(EVVO_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(evvo)

target_link_libraries(your-target PUBLIC evvo)
```

## Status

Core GA scaffolding (`Genome`, `Evolver`, `EvoPolicy`, `StdPolicy`,
mutation/crossover functions, the full set of callbacks above,
`GenerationStats`) is in place and demonstrated end-to-end by the
quadratic-fit example above. No automated tests exist for this core yet —
that's the next thing to add.
