#pragma once

#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>

#include "registry.hpp"

namespace metrics {

// Periodically writes the current state of all metrics to a file in its own thread.
// Should be explicitly started and stopped.
class MetricsWriter {
 public:
  MetricsWriter(const std::string& filename,
                std::chrono::milliseconds interval = std::chrono::seconds(1))
      : file_(filename, std::ios::app), delta_(interval) {}

  ~MetricsWriter() { Stop(); }

  void Start() {
    if (running_.exchange(true)) {
      return;
    }
    thread_ = std::thread([this] { WriteLoop(); });
  }

  void Stop() {
    if (!running_.exchange(false)) {
      return;
    }
    if (thread_.joinable()) {
      thread_.join();
    }
  }

 private:
  void WriteLoop() {
    auto next_wake = std::chrono::steady_clock::now();

    while (running_.load()) {
      auto now = std::chrono::system_clock::now();
      auto t = std::chrono::system_clock::to_time_t(now);
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now.time_since_epoch())
                    .count() %
                1000;

      std::ostringstream oss;
      oss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "."
          << std::setw(3) << std::setfill('0') << ms;

      for (auto m : GetMetrics()) {
        oss << " \"" << m->GetName() << "\" " << m->AggregateAndReset();
      }

      file_ << oss.str() << '\n';
      file_.flush();

      next_wake += delta_;
      std::this_thread::sleep_until(next_wake);
    }
  }

  std::ofstream file_;
  std::chrono::milliseconds delta_;
  std::atomic<bool> running_{false};
  std::thread thread_;
};

}  // namespace metrics
