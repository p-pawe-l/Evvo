# Evvo

A generic genetic-algorithm library, operating on populations of genomes of
any gene type, with crossover, mutation, and fitness evaluation supplied by
the caller.

## Language

**Genome**:
An individual's sequence of genes — the unit of selection, crossover, and
mutation. There is no separate "individual" wrapper; a Genome *is* a
population member.
_Avoid_: Individual, organism.

**Gene**:
A single value within a Genome.

**Population**:
The full set of Genomes making up one generation.

**Generation**:
One full step of the evolution loop: evaluate the population's fitness,
select parents, and produce the next population from them.

**GenerationStats**:
A snapshot of one Generation's results — its index, population, best/average
fitness, that generation's Generation Champion, and every individual's
fitness. Reported to every Callback after evaluation.

**Generation Champion**:
The best Genome found within a single Generation. May be worse than an
earlier generation's champion — nothing preserves it into the next
Population.
_Avoid_: best genome (ambiguous with Run Champion).

**Run Champion**:
The best Genome found across an entire run, tracked independently of any
single Population and returned by `Evolver::evolve()`.
_Avoid_: best genome (ambiguous with Generation Champion).

**Fitness**:
The score a Genome receives from the caller-supplied evaluation function.
Higher is better; selection strategies pick parents based on it.

**Crossover**:
Combines two parent Genomes into one new offspring Genome.

**Mutation**:
Perturbs a single Genome into a new, changed offspring Genome.

**Selector**:
The parent-selection strategy that picks which Genomes breed for a
generation (e.g. rank-based, roulette-wheel).

**Sampler**:
A gene-value source used to build an initial Population (e.g. drawing
uniformly from a range, or a fixed value).

**Evolver**:
Drives the evolution loop end-to-end via `evolve()`: repeatedly evaluates
the population, reports each Generation to callbacks, and asks the Policy
for the next Population, across the whole run.
_Avoid_: run, fit (settled on `evolve()` as the entry-point name).

**Policy**:
The strategy governing how one Generation becomes the next: selection,
crossover, and mutation applied together. Different Policy implementations
may choose different individuals for evolution — e.g. a future
`ElitismPolicy` carrying the Run Champion forward unchanged, versus
`StdPolicy` rebuilding the Population entirely from fresh selection and
crossover/mutation each Generation.
_Avoid_: Strategy.

**Callback**:
The single hook interface for reacting to a run, subclassed directly by
every concrete callback (no separate specialized wrapper type). Its `T` is
the gene type, consistently, across all three methods:
`call(const GenerationStats<T>&)` once per Generation,
`pre_run_call()` before the first Generation, and
`post_run_call(const Genome<T>& run_champion)` after the run ends.
`should_stop()` (default `false`) lets a callback request the run end
early after any generation.
_Avoid_: EvoCallback (merged into this).
