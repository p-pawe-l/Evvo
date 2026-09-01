# Coding Conventions

## Doc comments
Every public class/function gets a Doxygen-style comment: `@brief` one-liner,
`@tparam`/`@param`/`@return` as needed. Comments explain the WHY/contract,
not a restatement of the signature.

## Ownership and value semantics
Prefer passing/returning by value with move semantics over heap-allocated
wrapper types (e.g. `unique_ptr`). Only reach for indirection when there's
a concrete need — pointer stability, polymorphism — not by default.

## Strategy points are free functions or small interfaces
Pluggable behavior (crossover, mutation, sampling) is a free function with
a fixed signature, not a method on the type it operates on. Multi-strategy,
stateful behavior (Selector, Policy, Callback) is a small abstract interface
with one clear responsibility.

## Naming
Private members: `snake_case_` with a trailing underscore. Types: `PascalCase`.
Free functions and methods: `snake_case`.

## No speculative generality
Don't add parameters, hooks, or abstraction for behavior that isn't needed
yet (see docs/adr/ for decisions made this way, e.g. deferring callback
threading until there's a concrete slow callback).
