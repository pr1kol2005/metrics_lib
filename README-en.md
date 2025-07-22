# Metrics Library

## Available languages

1. [Russian](README.md)
2. [English](README-en.md)

## Overview

`metrics_lib` is a high-performance C++ library for collecting, aggregating, and periodically writing metrics to a text file. Key technologies used:

1. **Lock-free data structures**  
  `LockFreeQueue<T>` and `LockFreeStack<T>` allow recording data from worker threads without locks or race conditions, ensuring minimal latency.

2. **Hazard pointers**  
  A custom implementation ensures safe memory reclamation in lock-free containers without global locks or risk of use-after-free.

3. **Templated extensible architecture**  
  The `IMetric` interface defines a common contract for all metrics and allows storing instances of different types in a single container. The base classes `OrderedMetricBase<T>` and `UnorderedMetricBase<T>` implement buffering logic using lock-free structures, while concrete metrics (`CountMetric<T>`, `AverageMetric<T>`, etc.) inherit from them and implement only the `AggregateAndReset()` method responsible for aggregation logic. Users can add new metrics without modifying the core of the library by inheriting from the appropriate `<...>MetricBase<T>`.

4. **Registry and MetricsWriter**  
  The `Registry` singleton centrally stores all created metrics and manages their lifetime. `MetricsWriter` runs in a separate thread and every `delta` milliseconds reads, writes to a file, and resets accumulated values of all metrics. Output format:
  `yyyy-mm-dd hh:mm:ss.msmsms "name_1" <value_1> "name_2" <value_2> ...`.

### Requirement Compliance

- **Extensibility**: new metrics are added via inheritance and template registration — no changes to existing code required.  
- **Non-blocking writes**: the `SomeMetric::Record()` method does not use mutexes — all insertions go through a lock-free container.  
- **Automatic reset**: after each write, `MetricsWriter` clears metric buffers in preparation for the next measurement cycle.

## Project Structure

- **`.github/workflows/ci.yml`** — CI workflow that automatically builds, formats, and runs tests on every push/pull request.  
- **`examples/`** contains two usage examples (source code and output files):  
- **`cpu_and_http_metrics/`** — demonstrates `CountMetric<int>` and `AverageMetric<double>` for collecting average CPU usage and HTTP RPS;  
- **`custom_metrics/`** — demonstrates creating a custom metric `SequenceMetric<T>`.  
- **`external/`** — bundled GoogleTest for testing without external dependencies.  
- **`scripts/`** — shell scripts for building, formatting, and auto-fixing code.  
- **`src/`** is divided into modules:  
- **`hazard/`** — hazard pointer implementation;  
- **`lf/`** — lock-free queue and stack;  
- **`metrics/`** — `IMetric` interface, base classes `<...>MetricBase<T>`, built-in metrics, `Registry`, and `MetricsWriter`;  
- **`utils/`** — utility structures (`IntrusiveList<T>`).  
- **`tests/`** — unit and stress tests for all library components.

## Build & Run Instructions

```bash
# Clone the repository
git clone https://github.com/pr1kol2005/metrics_lib.git
cd metrics_lib

# Build the project with tests and examples enabled
cmake -B build -D BUILD_TESTS=ON -D BUILD_EXAMPLES=ON
cmake --build build

# Run example binaries
./bin/cpp_http
./bin/custom

# Run test binaries
./bin/lock_free_stack_stress
./bin/metrics_unit
