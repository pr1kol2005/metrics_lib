#pragma once

#include <string>

class IMetric {
 public:
  virtual ~IMetric() = default;

  virtual std::string AggregateAndReset() noexcept = 0;

  virtual const std::string& GetName() const noexcept = 0;
};
