# areo-evo

Evolution optimizes aerodynamics of jet planes.

The idea is to represent a jet shape as a genome, evolve a population of
shapes across generations, and score each shape by feeding it through an
aerodynamics evaluation function (e.g. lift/drag from a panel method or CFD
run). Over many generations, selection, crossover, and mutation push the
population toward better-performing shapes.

## Architecture

The genetic-algorithm core lives in `include/` and is header-only, generic
over the gene type `T` (e.g. a set of shape control points).

- **`genome.hpp` — `Genome<T>`**
  Owns a heap-allocated sequence of genes and exposes `mutate()` and
  `crossover()`, each driven by a caller-supplied function pointer that
  knows how to mutate/combine `T`.

- **`evo_policy.hpp` — `EvoPolicy<T>`**
  Abstract strategy interface for evolving a population. Implementations
  decide *how* selection, crossover, and mutation are applied. Holds the
  fitness function (set via `set_eval()`) and exposes:
  - `create_new_population(population)` — produces the next generation.
  - `choose_best(population)` — selects the fittest genome.
  - `get_total_fitness(population)` — sums fitness across a population.

- **`generation_stats.hpp` — `GenerationStats<T>`**
  A snapshot of one generation: generation index, population, best/average
  fitness, and the best genome found that generation.

- **`callback.hpp` / `evo_callback.hpp` — `Callback<T>` / `EvoCallback<T>`**
  `Callback<T>` is a generic interface invoked with an object of type `T`.
  `EvoCallback<T>` specializes it to `GenerationStats<T>`, so it's invoked
  once per generation with that generation's stats (for logging,
  checkpointing, plotting fitness over time, etc.).

- **`evolver.hpp` — `Evolver<T>`**
  Drives the evolution loop. Given an initial population, crossover/mutation
  functions, an `EvoPolicy<T>`, and an `EvoCallback<T>`, `run(runs, eval_func)`
  repeatedly scores the population, reports `GenerationStats<T>` to the
  callback, and asks the policy for the next generation — returning the best
  genome found across the whole run.

## Status

Core GA scaffolding (`Genome`, `Evolver`, `EvoPolicy`, `Callback`,
`GenerationStats`) is in place. Still to come: a concrete `EvoPolicy`
implementation (selection/crossover/mutation strategy), a jet-shape genome
representation, and the aerodynamics evaluation function that connects a
genome to a real fitness score.
