#include <lf/lock_free_stack.hpp>

#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>
#include <vector>

using lf::LockFreeStack;

TEST(LockFreeStackStressTest, ManyProducersConsumers) {
  constexpr int kNumProducers = 10;
  constexpr int kNumConsumers = 10;
  constexpr int kOpsPerProducer = 1'000'000;

  LockFreeStack<int> stack;

  std::atomic<long long> pushed_sum{0};

  std::vector<std::thread> producers;
  for (int p = 0; p < kNumProducers; ++p) {
    producers.emplace_back([&]() {
      long long local_sum = 0;
      for (int i = 1; i <= kOpsPerProducer; ++i) {
        stack.Push(i);
        local_sum += i;
      }
      pushed_sum.fetch_add(local_sum, std::memory_order_relaxed);
    });
  }

  std::atomic<int> pops_count{0};
  std::atomic<long long> popped_sum{0};
  const int total_pushes = kNumProducers * kOpsPerProducer;

  std::vector<std::thread> consumers;
  for (int c = 0; c < kNumConsumers; ++c) {
    consumers.emplace_back([&]() {
      while (true) {
        if (pops_count.load() >= total_pushes) {
          break;
        }

        auto opt = stack.TryPop();
        if (opt) {
          popped_sum.fetch_add(*opt, std::memory_order_relaxed);
          int prev = pops_count.fetch_add(1, std::memory_order_relaxed);
        } else {
          std::this_thread::yield();
        }
      }
    });
  }

  for (auto& t : producers) {
    t.join();
  }

  auto start = std::chrono::steady_clock::now();
  for (auto& t : consumers) {
    while (t.joinable()) {
      auto elapsed = std::chrono::steady_clock::now() - start;
      if (elapsed > std::chrono::seconds(60)) {
        FAIL() << "Timeout waiting for consumers — possible deadlock";
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(1));

      if (!t.joinable()) {
        break;
      }
      t.join();
    }
  }

  EXPECT_EQ(pops_count.load(), total_pushes)
      << "Expected " << total_pushes << " pops, got " << pops_count.load();

  EXPECT_EQ(popped_sum.load(), pushed_sum.load())
      << "Expected sum " << pushed_sum.load() << ", got " << popped_sum.load();
}
