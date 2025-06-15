#include <metrics/registry.hpp>
#include <metrics/writer.hpp>
#include <metrics/average_metric.hpp>
#include <metrics/count_metric.hpp>

#include <random>
#include <thread>
#include <chrono>

using namespace metrics;

int main() {
  auto& cpu1 = RegisterMetric<AverageMetric<double>>("CPU1");
  auto& cpu2 = RegisterMetric<AverageMetric<double>>("CPU2");
  auto& rps  = RegisterMetric<CountMetric<int>>("HTTP requests RPS");

  MetricsWriter writer("example.log");

  std::mt19937_64 rng{std::random_device{}()};
  std::uniform_real_distribution<double> cpuDist(0.0, 2.0);
  std::uniform_int_distribution<int>    rpsDist(0, 100);

  auto end_point = std::chrono::steady_clock::now() + std::chrono::seconds(15);

  writer.Start();

  std::vector<std::thread> workers;
  for (int i = 0; i < 4; ++i) {
    workers.emplace_back([&]() {
      while (std::chrono::steady_clock::now() < end_point) {
        cpu1.Record(cpuDist(rng));
        cpu2.Record(cpuDist(rng));
        rps.Record(rpsDist(rng));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    });
  }

  for (auto& t : workers) {
    t.join();
  }

  writer.Stop();
}
