#pragma once

#include <lf/lock_free_stack.hpp>

#include "metric_interface.hpp"

namespace metrics {

// Base class for thread safe metrics that don't need recorded values to be
// ordered. Uses a lock free stack that is more efficient than a lock free
// queue.
template <typename T>
class UnorderedMetricBase : public IMetric {
 public:
  explicit UnorderedMetricBase(std::string name) noexcept
      : name_(std::move(name)) {}

  void Record(T v) noexcept { buffer_.Push(std::move(v)); }

  const std::string& GetName() const noexcept final { return name_; }

 protected:
  lf::LockFreeStack<T>& Buffer() noexcept { return buffer_; }

 private:
  const std::string name_;
  lf::LockFreeStack<T> buffer_;
};

}  // namespace metrics
