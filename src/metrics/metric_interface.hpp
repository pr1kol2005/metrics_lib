#pragma once

#include <string>

namespace metrics {

// Base interface class for all metrics.
class IMetric {
 public:
  virtual ~IMetric() = default;

  virtual std::string AggregateAndReset() noexcept = 0;

  virtual const std::string& GetName() const noexcept = 0;
};

}  // namespace metrics
