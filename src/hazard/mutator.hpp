#pragma once

#include <array>
#include <atomic>

#include "fwd.hpp"
#include "limits.hpp"
#include "managed.hpp"
#include "owner.hpp"

namespace hazard {

class Mutator {
 public:
  explicit Mutator(Collector* collector) : collector_(collector) {}

  PtrOwner GetHazardPtr(size_t index) {
    return PtrOwner{&(hazard_ptrs_[index])};
  }

  void Gc();

  template <typename T>
  void Retire(T* obj) {
    static_assert(IsManaged<T>);

    retired_.PushBack(obj);

    if (++retired_size_ >= kRetireBatchSize) {
      Gc();
    }
  }

  ~Mutator() { Gc(); }

  std::atomic<Mutator*> next_{nullptr};

 private:
  [[maybe_unused]] Collector* collector_;

  utils::IntrusiveList<ManagedBase> retired_{};
  size_t retired_size_ = 0;

  std::array<PtrRecord, kNumHazardPtrsPerThread> hazard_ptrs_{};
};

}  // namespace hazard
