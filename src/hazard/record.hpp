#pragma once

#include <atomic>

namespace hazard {

struct PtrRecord {
  PtrRecord() = default;

  void Set(void* obj) { ptr_.store(obj); }

  void* Get() { return ptr_.load(); }

  void Reset() { ptr_.store(nullptr); }

 private:
  // Actual hazard pointer.
  std::atomic<void*> ptr_{nullptr};
};

}  // namespace hazard
