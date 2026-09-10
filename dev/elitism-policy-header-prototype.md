# Elitism policy header (prototype)

2026-09-02

## What was done

Filled in `include/core/elitism_policy.hpp` (previously just `#pragma once`)
with a prototype declaration of `ElitismPolicy : public EvoPolicy<double>`.
`CONTEXT.md` already names this type as the intended alternative to
`StdPolicy`: it carries the Run Champion (more generally, the
`elite_count_` fittest individuals) forward into the next generation
unchanged, instead of rebuilding the whole population from fresh
selection/crossover/mutation each generation.

The shape mirrors `StdPolicy` (`include/core/std_policy.hpp`) closely —
same `choose_parents()`/`roll()` private helpers — plus:

- `elite_count_`, defaulted to `1`, settable via the constructor or
  `set_elite_count()`.
- `copy_elites()`, a private helper that ranks `prev` by `eval.fitnesses`
  and copies the top `elite_count_` individuals into the new population
  before the rest is filled the `StdPolicy` way.

Only the header was implemented — `src/core/elitism_policy.cpp` stays
empty for now, and the policy isn't wired into `Evolver` yet.

## Alternative paths considered

- Ranking elites via `PopulationEval` directly: rejected because
  `PopulationEval` only tracks a single `best_index`/`best_fitness`, not a
  ranked list — `copy_elites()` has to sort indices by `eval.fitnesses`
  itself.
- Making `elite_count_` a required constructor argument instead of
  defaulting to `1`: kept the default so `ElitismPolicy()` stays usable
  the same way `StdPolicy()` is, without forcing every call site to pick a
  number up front.

## Potential problems

- `elite_count_ >= population size` isn't validated at the header level;
  `create_new_population()` will need an `assert` (see `StdPolicy`'s
  `assert(selector_ != nullptr ...)` pattern) once implemented.
- Elites are copied by value each generation, which is consistent with
  `Genome<T>` being a plain `std::vector<T>` (see
  `docs/adr/0001-genome-by-value.md`) but is still an extra copy per elite
  per generation worth keeping in mind if `elite_count_` grows large.

## Advantages

- Guarantees fitness is monotonically non-decreasing across generations
  (the current best individuals can't be lost to selection variance).
- Reuses `StdPolicy`'s parent-selection/crossover/mutation logic for the
  non-elite remainder, so the two policies stay easy to compare.
