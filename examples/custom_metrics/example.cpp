#include <metrics/registry.hpp>
#include <metrics/writer.hpp>
#include <metrics/ordered_metric_base.hpp>

#include <array>
#include <atomic>
#include <thread>
#include <chrono>
#include <cstdint>
#include <sstream>

using namespace metrics;

template <typename T>
class SequenceMetric : public OrderedMetricBase<T> {
 public:
  using OrderedMetricBase<T>::OrderedMetricBase;

  std::string AggregateAndReset() noexcept override {
    std::ostringstream oss;
    oss << "[";
    bool first = true;
    while (auto opt = this->Buffer().TryPop()) {
      if (!first) oss << ", ";
      oss << *opt;
      first = false;
    }
    oss << "]";
    return oss.str();
  }
};

int main() {
  auto& seq_even = RegisterMetric<SequenceMetric<uint32_t>>("Even");
  auto& seq_odd = RegisterMetric<SequenceMetric<uint32_t>>("Odd");

  MetricsWriter writer("example.log");

  auto end_time = std::chrono::steady_clock::now() + std::chrono::seconds(15);

  std::array<std::atomic<uint32_t>, 4> counters = {0, 1000, 2000, 3000};

  writer.Start();

  std::vector<std::thread> workers;
  for (size_t i = 0; i < 4; ++i) {
    workers.emplace_back([index = i, &counters, &seq_even, &seq_odd, end_time]() {
      while (std::chrono::steady_clock::now() < end_time) {
        counters[index].fetch_add(1);

        uint32_t value = counters[index].load();

        if (value % 2 == 0) {
          seq_even.Record(value);
        } else if (value % 2 == 1) {
          seq_odd.Record(value);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
      }
    });
  }

  for (auto& t : workers) {
    t.join();
  }

  writer.Stop();
}
