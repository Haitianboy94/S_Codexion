*This project has been created as part of the 42 curriculum by rulouis.*

# Codexion

> Master the race for resources before the deadline masters you

## Description

Codexion is a concurrency simulation in C where multiple coders compete for a limited number of USB dongles to compile their quantum code. Each coder is represented by a POSIX thread and must acquire two dongles simultaneously before compiling. After compiling, they debug and refactor before attempting to compile again.

The simulation stops either when a coder burns out (fails to start compiling within `time_to_burnout` milliseconds) or when all coders have compiled at least `number_of_compiles_required` times.

The project explores real-world concurrency challenges: deadlock prevention, starvation avoidance, fair resource scheduling, and precise timing — all implemented from scratch using POSIX threads and mutexes.

---

## Instructions

### Compilation

```bash
make
```

This produces the `codexion` executable.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory.

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Milliseconds before a coder burns out without compiling |
| `time_to_compile` | Milliseconds to compile (requires two dongles) |
| `time_to_debug` | Milliseconds to debug |
| `time_to_refactor` | Milliseconds to refactor |
| `number_of_compiles_required` | Stop when all coders reach this compile count |
| `dongle_cooldown` | Milliseconds a dongle is unavailable after release |
| `scheduler` | `fifo` (First In First Out) or `edf` (Earliest Deadline First) |

### Example runs

```bash
# 5 coders, 800ms burnout, 200ms compile/debug/refactor, 3 compiles required
./codexion 5 800 200 200 200 3 50 fifo
./codexion 5 800 200 200 200 3 50 edf

# Single coder
./codexion 1 800 200 200 200 3 50 fifo

# Burnout scenario
./codexion 5 200 800 200 200 3 50 fifo
```

# Memory leaks checks
valgrind --leak-check=full ./codexion 2 800 200 200 200 3 50 fifo

### ThreadSanitizer (race detection)

To build with ThreadSanitizer for data race detection:

```bash
make san
./codexion_san <args>
```

> **Note:** The only reported data race involves reading `stop_flag` inside
> `dongle_take` while holding `d->mutex`. This race is structurally
> unavoidable without C11 atomics (not in the allowed function list) —
> locking `state_mutex` there would create a deadlock with the monitor.
> `volatile int` ensures correct runtime behavior on x86. This pattern
> is standard in 42 POSIX thread projects.

### Cleanup

```bash
make clean    # Remove object files
make fclean   # Remove object files and binary
make re       # Full rebuild
```

---

## Blocking Cases Handled

### 1 — Deadlock prevention (Coffman's conditions)

With N coders in a circle each needing two dongles, a naive implementation causes deadlock: every coder takes their left dongle and waits forever for their right dongle, which their neighbor holds.

**Solution:** odd-numbered coders take left then right; even-numbered coders take right then left. This breaks the circular wait condition, one of the four Coffman conditions required for deadlock, and guarantees at least one coder can always proceed.

### 2 — Starvation prevention

Under FIFO scheduling, a coder that repeatedly loses the dongle race could starve. The priority heap ensures requests are served strictly in arrival order — no coder can be bypassed indefinitely.

Under EDF scheduling, the coder whose burnout deadline is nearest is served first, which by definition prevents any coder from missing their deadline as long as the parameters are feasible.

### 3 — Dongle cooldown handling

After a dongle is released, it is marked unavailable until `dongle_cooldown` milliseconds have elapsed (`available_at_ms = now_ms() + cooldown`). The `dongle_take` wait loop checks this timestamp on every wake-up before granting access.

### 4 — Precise burnout detection

A dedicated monitor thread polls every 1ms and checks each coder's elapsed time since their last compile start. When `elapsed > time_to_burnout` and the coder is not currently compiling, burnout is logged and `stop_flag` is set. The burnout log is guaranteed to appear within 10ms of the actual burnout time.

The `state_mutex` protects both the `last_compile_start_ms` reset in coder threads and the burnout check in the monitor, eliminating race conditions between the two.

### 5 — Log serialization

All output goes through `log_event`, which locks `log_mutex` before writing. This ensures no two messages ever interleave on a single line, regardless of how many threads call it simultaneously. The mutex is unlocked before returning, keeping the critical section as short as possible.

### 6 — Stop flag propagation

When `stop_flag` is set (by burnout or completion), all dongle condition variables are broadcast immediately. This wakes any coder blocked in `dongle_take`, which checks `stop_flag` at the top of its wait loop and exits cleanly. Coders also check `stop_flag` between each phase (after compiling, debugging, refactoring) via `interruptible_sleep`, which polls every 1ms.

---

## Thread Synchronization Mechanisms

### Mutexes used

| Mutex | Purpose |
|---|---|
| `d->mutex` (per dongle) | Protects `held_by`, `available_at_ms`, and the priority queue |
| `sim->log_mutex` | Serializes all output — only one thread writes at a time |
| `sim->state_mutex` | Protects `last_compile_start_ms`, `state`, and `compile_count` for all coders |

### Condition variables

Each dongle has a `pthread_cond_t cond`. Coders waiting for a dongle sleep on this condition variable using `pthread_cond_timedwait` with a 1ms timeout. This allows:
- Periodic re-checking of `stop_flag` without busy-waiting
- Immediate wake-up when `dongle_put` calls `pthread_cond_broadcast`
- Immediate wake-up when the monitor sets `stop_flag` and broadcasts

### Lock ordering

To prevent deadlock between mutexes, a strict lock order is enforced:
1. `d->mutex` (dongle) — acquired and released inside `dongle_take` / `dongle_put`
2. `sim->state_mutex` — acquired in coder threads and monitor, never while holding `d->mutex`
3. `sim->log_mutex` — acquired in `log_event`, never while holding other locks

`log_event` is always called **after** releasing `d->mutex` and `state_mutex`, ensuring no circular wait between locks.

### Race condition prevention

**`last_compile_start_ms` race:** The monitor reads this field to detect burnout. The coder resets it at the start of each compile. Both operations are protected by `state_mutex`, ensuring the monitor never reads a stale value mid-reset.

**`compile_count` race:** Written by coder threads under `state_mutex`. Read by the monitor's `all_done` check also under `state_mutex`. This guarantees a consistent view of all counts when checking for completion.

**`stop_flag` race:** Written atomically as an `int`. Read without locking in hot paths (`interruptible_sleep`, loop conditions) as a soft signal — safe for single-writer, multiple-reader patterns on modern architectures. Written under `state_mutex` in the monitor to coordinate with `compile_count` reads.

### How coder and monitor communicate

```
Coder thread:
  lock(state_mutex) → reset last_compile_start_ms, set state = COMPILING
  unlock(state_mutex)
  → compile (interruptible_sleep checks stop_flag every 1ms)
  lock(state_mutex) → increment compile_count, set state = DEBUGGING
  unlock(state_mutex)

Monitor thread (every 1ms):
  lock(state_mutex)
  → check each coder: state != COMPILING && elapsed > time_to_burnout → burnout
  → check all_done → all compile_count >= required → stop
  unlock(state_mutex)
  → if stopping: broadcast all dongle conds to wake blocked coders
```

---

## Resources

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [The Little Book of Semaphores — Allen B. Downey](https://greenteapress.com/wp/semaphore/)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [pthread_cond_timedwait — Linux man page](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html)
- [Coffman Conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Coffman_conditions)

### AI usage

Claude (Anthropic) was used throughout this project as a learning and debugging aid:
- **Concept explanation:** understanding `gettimeofday`, mutex/condvar patterns, heap data structures, and EDF scheduling theory
- **Code review:** identifying bugs in lock ordering, race conditions, and cleanup logic
- **Debugging:** tracing deadlocks, false burnouts, and stopping condition failures through iterative output analysis
- All generated or suggested code was reviewed, understood, and validated before use.