#pragma once

#include <vector>

#include "metric_interface.hpp"

namespace metrics {

// Singleton class that keeps track of all registered metrics.
class Registry {
 public:
  static Registry& Instance() {
    static Registry instance;
    return instance;
  }

  Registry(const Registry&) = delete;
  Registry& operator=(const Registry&) = delete;
  Registry(Registry&&) = delete;
  Registry& operator=(Registry&&) = delete;

  // Deregistration happens only once in the destructor.
  ~Registry() {
    for (auto* metric : metrics_) {
      delete metric;
    }
    metrics_.clear();
  }

  template <typename MetricType, typename... Args>
  MetricType& Register(Args&&... args) {
    auto* metric = new MetricType(std::forward<Args>(args)...);
    {
      // Registration may happen concurrently.
      std::lock_guard<std::mutex> lock(mutex_);
      metrics_.push_back(metric);
    }
    return *metric;
  }

  std::vector<IMetric*>& GetMetrics() { return metrics_; }

 private:
  Registry() = default;

  mutable std::mutex mutex_;
  // std::vector is better than utils::IntrusiveList because of its contiguous
  // memory layout.
  std::vector<IMetric*> metrics_;
};

inline std::vector<IMetric*>& GetMetrics() {
  return Registry::Instance().GetMetrics();
}

template <typename MetricType, typename... Args>
MetricType& RegisterMetric(Args&&... args) {
  return Registry::Instance().Register<MetricType>(std::forward<Args>(args)...);
}

}  // namespace metrics
