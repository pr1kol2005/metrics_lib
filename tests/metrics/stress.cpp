#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "metrics/count_metric.hpp"
#include "metrics/registry.hpp"

using namespace metrics;

class RegistryStressTest : public ::testing::Test {
 protected:
  void SetUp() override {
    metric_ = std::make_unique<CountMetric<int>>("stress_count");
  }
  void TearDown() override { metric_.reset(); }

  std::unique_ptr<CountMetric<int>> metric_;
};

TEST_F(RegistryStressTest, ConcurrentRecordingSingleMetric) {
  constexpr int kThreads = 10;
  constexpr int kOps = 1'000'000;

  auto& m = *metric_;
  std::vector<std::thread> threads;
  for (int i = 0; i < kThreads; ++i) {
    threads.emplace_back([&m]() {
      for (int j = 0; j < kOps; ++j) {
        m.Record(1);
      }
    });
  }
  for (auto& t : threads) {
    t.join();
  }

  EXPECT_EQ(std::stoi(m.AggregateAndReset()), kThreads * kOps);
  EXPECT_EQ(std::stoi(m.AggregateAndReset()), 0);
}

TEST(RegistryMultipleStressTest, ConcurrentRecordingMultipleMetrics) {
  constexpr int kThreads = 10;
  constexpr int kOps = 1'000'000;

  auto& r = Registry::Instance();
  std::vector<CountMetric<int>*> metrics;
  for (int i = 0; i < kThreads; ++i) {
    metrics.push_back(
        &r.Register<CountMetric<int>>("stress_" + std::to_string(i)));
  }

  std::vector<std::thread> threads;
  for (int i = 0; i < kThreads; ++i) {
    threads.emplace_back([m = metrics[i]]() {
      for (int j = 0; j < kOps; ++j) {
        m->Record(1);
      }
    });
  }
  for (auto& t : threads) {
    t.join();
  }

  for (auto* m : metrics) {
    EXPECT_EQ(std::stoi(m->AggregateAndReset()), kOps);
  }
}
