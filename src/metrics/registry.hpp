#pragma once

#include <vector>

#include "metric_interface.hpp"

namespace metrics {

// Singleton
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

  ~Registry() {
    for (auto* metric : metrics_) {
      delete metric;
    }
    metrics_.clear();
  }

  template <typename MetricType, typename... Args>
  MetricType& Register(Args&&... args) {
    auto* metric = new MetricType(std::forward<Args>(args)...);
    metrics_.push_back(metric);
    return *metric;
  }

  std::vector<IMetric*>& GetMetrics() { return metrics_; }

 private:
  Registry() = default;

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
