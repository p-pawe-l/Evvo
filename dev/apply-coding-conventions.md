# Applied CODING_CONVENTIONS.md across the repo

2026-09-02

## What was done

Brought the existing codebase in line with the rewritten `CODING_CONVENTIONS.md`,
rather than letting old code drift while only new code follows it:

- Stripped `/** @file ... @brief ... */` headers and most `@param`/`@tparam`/`@return`
  Doxygen blocks across every header and source file. Kept only short "why"
  comments where the reasoning genuinely isn't obvious from the code (e.g. why
  `Genome<T>` is a plain `std::vector<T>` alias, why `RandomSampler`'s `k`
  coefficient behaves the way it does).
- Split `Evolver<T>::evolve()` — which mixed run-bookkeeping, the generation
  loop, and callback dispatch in one function — into named private helpers:
  `notify_pre_run()`, `notify_post_run()`, `notify_generation()`,
  `update_run_champion()`.
- Applied a few genuine C++20 idiom improvements: `std::ranges::for_each`/
  `generate` in `mut_func.hpp` instead of index loops, `std::ranges::max_element`
  + `std::accumulate` in `StdPolicy::evaluate()` instead of a fused
  best/total-tracking loop, `std::ranges::minmax` in `StatsSummaryCallback`,
  and plain range-based `for` instead of `->data()[i]` indexing in the
  print/save/checkpoint callbacks.
- Ran `clang-format -i` repo-wide, then `clang-tidy` and fixed the clear-cut
  findings: missing `override` on `~StdPolicy()`, missing `[[nodiscard]]` on
  every `Sampler::get_sample()`, `.size() == 0` → `.empty()`, `->data()[i]` →
  `(*ptr)[i]` in `main.cpp`, and `std::move`-ing the `std::function` params
  in `EvoPolicy`/`Evolver` that were only copied once.
- Fixed a real bug found while reading `RankSelector::build_from_eval()` for
  this pass (not a style issue, but surfaced by the same read-through):
  `order_.resize(n)` followed by `n` more `emplace_back()` calls left `order_`
  with `2n` entries instead of `n`, and never cleared it between generations,
  so selection weights were built over corrupted, ever-growing data.

  ```cpp
  // before
  order_.resize(n);
  for (std::size_t i = 0; i < n; ++i) {
      order_.emplace_back(i, eval.fitnesses[i]);
  }

  // after
  order_.clear();
  order_.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
      order_.emplace_back(i, eval.fitnesses[i]);
  }
  ```

## Why

`CODING_CONVENTIONS.md` was rewritten with a different philosophy than the
code that existed before it: lighter docs (code/naming carries meaning,
comments explain *why* not *what*), functions split on "AND", and a
formatting/linting discipline. Applying it only to future code would leave
the repo permanently split between two styles.

## Other paths considered

- Could have applied this incrementally as each file was next touched for
  unrelated work, instead of one repo-wide sweep — lower risk per change, but
  leaves the codebase inconsistent for an unknown amount of time.
- Could have used `clang-tidy --fix` for automated mechanical fixes instead of
  hand-editing — faster, but riskier to review since it can touch more than
  intended in one pass.
- The `RankSelector` bug fix could have been its own separate commit/PR
  instead of bundled into a docs/style pass — cleaner git history, but it was
  found *by* this pass (reading every function to decide what doc comments
  to keep), so splitting it out would have meant re-finding it later.

## Problems

- Bundling a real behavior-changing bug fix into a style/docs pass mixes
  concerns in the git history — anyone bisecting a regression has to know to
  look here.
- `std::move`-ing the `std::function` constructor/setter parameters changes
  `EvoPolicy`/`Evolver`'s public API surface slightly (callers passing a
  named lvalue no longer see it usable afterward the same way) — not
  currently used anywhere that matters, but worth knowing.
- Several `clang-tidy` findings were deliberately left unresolved
  (`bugprone-easily-swappable-parameters` on things like `mutation_prob`/
  `crossover_prob`, `modernize-use-designated-initializers`) and will keep
  showing up in future lint runs until someone decides to act on them.

## Advantages

- Docs now say *why* instead of restating signatures — less to keep in sync
  as the code changes.
- `Evolver::evolve()` reads as a sequence of named steps instead of one long
  function.
- The `RankSelector` fix means the library's flagship rank-based selector
  actually works — the quadratic-fit demo went from a fitness plateau around
  0.0001 (nowhere near converged) to 0.999999 (essentially exact) with the
  same code, same generations, same everything else.
