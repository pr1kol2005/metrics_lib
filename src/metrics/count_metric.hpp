#pragma once

#include "unordered_metric_base.hpp"

namespace metrics {

// Implements a metric that records the sum of event's values.
template <typename T>
class CountMetric : public UnorderedMetricBase<T> {
 public:
  using UnorderedMetricBase<T>::UnorderedMetricBase;

  std::string AggregateAndReset() noexcept final {
    T sum = 0;
    while (auto opt = this->Buffer().TryPop()) {
      sum += *opt;
    }
    return std::to_string(sum);
  }
};

}  // namespace metrics
