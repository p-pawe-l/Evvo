# Callback threading is deferred; no queue/worker-thread support in the base class

Expensive callback work (heavy logging, file I/O) can stall the main
evolution loop, since every `Callback::call()` runs synchronously on the
thread driving `Evolver::evolve()`. A queue-plus-worker-thread pattern
(push work in `call()`, drain it on a separate thread) solves this, but
adding it to `Callback` itself means designing thread lifecycle, exception
handling across the queue, and shutdown ordering — a separate problem from
getting the core evolve loop and callback hooks working.

For now, `Callback` stays synchronous and single-threaded, with no built-in
queueing. A callback that needs to offload expensive work can implement the
queue-and-worker-thread pattern entirely inside its own `call()` override,
with zero changes to the `Callback` base class. Revisit a built-in version
once there's a concrete callback slow enough to need it.
