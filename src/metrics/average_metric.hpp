#pragma once

#include "unordered_metric_base.hpp"

template <typename T>
class AverageMetric : public UnorderedMetricBase<T> {
 public:
  using UnorderedMetricBase<T>::UnorderedMetricBase;

  std::string AggregateAndReset() noexcept final {
    T sum{};
    int count = 0;
    while (auto opt = this->Buffer().TryPop()) {
      sum += *opt;
      count++;
    }
    T avg = count ? sum / count : T{};
    return std::to_string(avg);
  }
};
