#pragma once

#include "ordered_metric_base.hpp"

template <typename T>
class AverageMetric : public OrderedMetricBase<T> {
 public:
  using OrderedMetricBase<T>::OrderedMetricBase;

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
