# Genome is stored and passed by value, not through an owning pointer

`Genome<T>` used to be wrapped in `IndPtr<T> = std::unique_ptr<Genome<T>>`
for every population member, and `PopulationVec<T>` was
`std::vector<IndPtr<T>>`. Now that `Genome<T>` is a plain `std::vector<T>`
alias, that wrapper would cost two heap allocations per individual per
generation (one for the `Genome` object via `make_unique`, one for its
internal buffer) instead of one, with no compensating benefit — nothing in
this codebase needs pointer stability into the population or polymorphic
individuals. `PopulationVec<T>` is now `std::vector<Genome<T>>`, and
crossover/mutation functions take and return `Genome<T>` by value, relying
on move semantics.
