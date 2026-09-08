# Cache prototype

2026-09-02

## What was done

Added `include/util/cache.hpp`, a header-only `Cache<T>` template (header-
only because it's a template class, same reason `util/rand_util.hpp` is
header-only). It keeps the `capacity_` highest-scoring values it's given,
ranked best-first, as `CacheEntry<T>` pairs (`value` + the `score` it was
cached at).

This started as an `EliteCache<T>` coupled to `Genome<T>`/`PopulationEval`
(see git history), but was rewritten as a general-purpose `Cache<T>` before
anything used it — it now takes plain `std::vector<T>`/`std::vector<double>`
and has no dependency on `core/`. Internally still split per
`CODING_CONVENTIONS.md`'s "split on AND" rule: `populate_from()` (collect),
`sort_best_first()` (rank), `trim_to_capacity()` (bound).

This is a prototype: nothing constructs or calls `Cache` yet. It's meant as
the caching piece `ElitismPolicy::copy_elites()` (see
`dev/elitism-policy-header-prototype.md`) can be built on — e.g. via
`Cache<Genome<T>>` — instead of `copy_elites()` re-sorting the whole
population itself each generation.

## Alternative paths considered

- Keeping it as `EliteCache<T>` tied to `PopulationEval`: simpler call
  site for `ElitismPolicy` specifically, but ties a caching utility to one
  domain type for no real gain — a plain `std::vector<double>` of scores
  is exactly as easy to pass in, and the generic version is reusable
  anywhere a "keep the top N" cache is needed.

## Potential problems

- `update()` copies every value into `entries_` before sorting and
  trimming, i.e. `O(values.size())` copies per call even though only
  `capacity_` survive. Fine for a prototype; a partial-sort or a min-heap
  of size `capacity_` would avoid the wasted copies if this becomes a hot
  path.
- No validation that `scores.size() == values.size()`, or that
  `capacity_ > 0` — left out on purpose for now, matching how the sibling
  `ElitismPolicy` prototype header also defers its own validation.

## Advantages

- Fully generic and decoupled from `core/`, so it isn't limited to caching
  elite `Genome<T>`s — any "keep the top N by score" use case can reuse it.
- Ranking/trimming logic is isolated and independently testable, rather
  than living inside `ElitismPolicy` itself.
