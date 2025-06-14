#pragma once

#include "managed.hpp"
#include "record.hpp"
#include "reset_guard.hpp"

namespace hazard {

class PtrOwner {
  friend class Mutator;

 public:
  // Non-copyable
  PtrOwner(const PtrOwner&) = delete;
  PtrOwner& operator=(const PtrOwner&) = delete;

  template <typename T>
  T* Protect(std::atomic<T*>& atomic_ptr) {
    static_assert(IsManaged<T>);

    T* ptr;
    do {
      ptr = atomic_ptr.load();
      hazard_ptr_->Set(ptr);
    } while (ptr != atomic_ptr.load(std::memory_order_acquire));

    assert(hazard_ptr_->Get() == atomic_ptr.load());

    return ptr;
  }

  template <typename T>
  void Set(T* obj) {
    static_assert(IsManaged<T>);

    hazard_ptr_->Set(obj);
  }

  void Reset() { hazard_ptr_->Reset(); }

  auto ScopedReset() { return PtrResetGuard{*this}; }

  ~PtrOwner() { Reset(); }

 private:
  explicit PtrOwner(PtrRecord* hp) : hazard_ptr_(hp) {}

 private:
  [[maybe_unused]] PtrRecord* hazard_ptr_;
};

}  // namespace hazard
