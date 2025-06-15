#pragma once

#include <lf/lock_free_queue.hpp>

#include "metric_interface.hpp"

namespace metrics {

template <typename T>
class OrderedMetricBase : public IMetric {
 public:
  explicit OrderedMetricBase(std::string name) noexcept
      : name_(std::move(name)) {}

  void Record(T v) noexcept { buffer_.Push(std::move(v)); }

  const std::string& GetName() const noexcept final { return name_; }

 protected:
  lf::LockFreeQueue<T>& Buffer() noexcept { return buffer_; }

 private:
  const std::string name_;
  lf::LockFreeQueue<T> buffer_;
};

}  // namespace metrics
