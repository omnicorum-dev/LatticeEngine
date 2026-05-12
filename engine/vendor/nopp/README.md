# nopp

A single-header C++ utility library for build systems, logging, file system operations, process management, and benchmarking.

The Process / Build System is basically stolen from Tsoding's [nob.h](https://github.com/tsoding/nob.h), but adapted to use C++ paradigms.

---

## Installation

Copy `nopp.h` into your project. In **exactly one** `.cpp` file, define the implementation macro before including the header:

```cpp
#define NOPP_IMPLEMENTATION
#include "nopp.h"
```

All other files can include the header normally:

```cpp
#include "nopp.h"
```

Requires **C++20** (uses `std::span`, `std::ranges`, and `auto` parameters in lambdas).
Although I'm thinking about going through and making this library C++17 compatible, or maybe even C++14 if I'm feeling spicy.

---

## Modules

### Primitive Type Aliases

nopp exposes short, explicit type aliases to avoid ambiguity across platforms:

```cpp
i8, i16, i32, i64   // signed integers
u8, u16, u32, u64   // unsigned integers
f32, f64            // float / double
```

Common constants are also provided: `pi_f32`, `tau_f64`, `e_f64`, `gold_big_f32`, `machine_epsilon_f64`, min/max values for all integer types, etc.

### Logging

A lightweight, colored, leveled logger that writes to `stderr` and optionally to a log file.

```cpp
nopp::log_path = "build.log"; // optional: also write to a file

LOG_INFO("Building {} files", count);
LOG_WARN("Missing optional dependency: {}", name);
LOG_ERROR("Failed to open: {}", path);
LOG_FATAL("Unrecoverable error");
LOG_DEBUG("Value of x = {}", x);
LOG_TRACE("Entering function");
```

**Log levels** (in decreasing severity): `FATAL`, `ERROR`, `WARN`, `INFO`, `DEBUG`, `TRACE`.

**Compile-time options:**

| Macro | Effect |
|---|---|
| `LOG_INCLUDE_LINE_INFO` | Prepend `file:line` to WARN/INFO/DEBUG/TRACE (always on for FATAL/ERROR) |
| `LOG_STRIP` | Strip all log macros from the binary |
| `LOG_STRIP_DEBUG` | Strip only DEBUG (combine as needed per level) |

**Assertions and guards:**

```cpp
LOG_ASSERT(ptr != nullptr, "Pointer must not be null");
UNREACHABLE(); // logs + terminates if executed
```

All log events are also stored in `nopp::log_handler` (a `std::vector<LogEvent>`) for programmatic inspection.

### Formatted Printing

A format-style print system compatible with C++17, supporting alignment, padding, numeric bases, and precision.
A big bonus is that you can easily provide your own printing protocol for a struct by overloading the `operator<<`.
No insane fmt stuff like in C++23.

```cpp
nopp::println("Hello, {}!", name);
nopp::print(std::cerr, "Error code: {:08x}", code);
std::string s = nopp::stringPrint("{:>10.3f}", value);
```

**Format spec syntax** (inside `{}`):

```
{[fill]align][sign][width][.precision][type]}
```

| Specifier | Example | Meaning |
|---|---|---|
| `>`, `<`, `^` | `{:>10}` | Right / left / center align |
| `+` | `{:+}` | Always show sign |
| `08` | `{:08d}` | Zero-pad to width 8 |
| `.3f` | `{:.3f}` | Fixed-point, 3 decimal places |
| `x` / `X` | `{:x}` | Lowercase / uppercase hex |
| `o` | `{:o}` | Octal |
| `e` / `E` | `{:e}` | Scientific notation |

Escaped braces: `{{` → `{`, `}}` → `}`.

### File System

Thin wrappers around `std::filesystem` with error logging built in:

```cpp
nopp::mkdir_if_not_exists("build/obj");
nopp::delete_file("build/old.o");
nopp::copy_directory_recursive("assets", "dist/assets");
nopp::file_exists("CMakeLists.txt");
nopp::get_file_type("src/main.cpp");

nopp::path cwd = nopp::get_current_dir();
nopp::set_current_dir("subproject");

nopp::path exe = nopp::running_executable_path(); // cross-platform
```

**Directory traversal:**

```cpp
// Flat listing
std::vector<nopp::path> entries = nopp::read_dir("src");

// Recursive walk with control flow
nopp::walk_dir("src", [](const auto& entry) {
    if (entry.path().filename() == ".git")
        return nopp::WalkAction::Skip;
    std::cout << entry.path() << '\n';
    return nopp::WalkAction::Cont;
});
```

`WalkAction` values: `Cont` (continue), `Skip` (skip into directory), `Stop` (stop walk entirely).

**Incremental build helper:**

```cpp
std::vector<nopp::path> inputs = { "main.cpp", "util.cpp" };
if (nopp::needs_rebuild("build/app", inputs)) {
    // recompile
}
```

Returns `true` if the output is missing or any input is newer than the output.

### Process / Build System

Spawn and manage child processes, build command lines, and parallelize work.

```cpp
nopp::Cmd cmd;
cmd.append("c++", "-std=c++20", "-O2", "-o", "app", "main.cpp");

// Synchronous — blocks until done
bool ok = cmd.run();

// Async — fire and forget into a pool
nopp::ProcessList pool;
nopp::RunOpts opts{ .async = &pool, .max_procs = 8 };
cmd.run(opts);
nopp::procs_wait(pool); // drain remaining

// Query logical CPU count
int cores = nopp::num_procs();

// Inspect a running process
auto proc = nopp::spawn(cmd);
auto result = proc->poll(); // PollResult::Done / Running / Error
std::move(*proc).wait();    // block until exit, returns true on code 0
```

**Self-rebuilding binaries** (build-system pattern):

```cpp
int main(int argc, char** argv) {
    NOPP_REBUILD_URSELF(argc, argv);
    // ... rest of build script
}
```

If the source file is newer than the compiled binary, it recompiles itself and re-executes transparently — inspired by the [nob.h](https://github.com/tsoding/nob.h) pattern.

### Benchmarking & Timing

**Scope-based profiling** (outputs Chrome tracing JSON):

```cpp
BENCHMARK.BeginSession("my_session", "profile.json");

void expensive_function() {
    PROFILE_FUNCTION(); // times the entire function
    {
        PROFILE_SCOPE("inner loop");
        // ...
    }
}

BENCHMARK.EndSession();
// Open profile.json in chrome://tracing
```

Define `STRIP_PROFILING` to compile out all profiling macros with zero overhead.

**Scoped timer** (logs to INFO on destruction):

```cpp
{
    nopp::ScopedTimer t("data load");
    load_data();
} // prints: "[INFO] data load took 42.317 ms"
```

**Manual timer:**

```cpp
nopp::Timer t;
do_work();
double ms  = t.elapsed_ms();
double sec = t.elapsed_sec();
t.reset();
```

---

## Quick Example

```cpp
#define NOPP_IMPLEMENTATION
#include "nopp.h"

int main(int argc, char** argv) {
    NOPP_REBUILD_URSELF(argc, argv);

    LOG_INFO("Starting build");
    nopp::mkdir_if_not_exists("build");

    nopp::ProcessList pool;
    nopp::RunOpts opts{ .async = &pool, .max_procs = nopp::num_procs() };

    for (auto& src : nopp::read_dir("src")) {
        nopp::Cmd cmd;
        cmd.append("c++", "-std=c++20", "-c", src.string(), "-o",
                   ("build/" + src.stem().string() + ".o"));
        cmd.run(opts);
    }

    nopp::procs_wait(pool);
    LOG_INFO("Done");
}
```

---

## License

Do whatever you want with it.