#pragma once

#include <atomic>

#include "mutator.hpp"

namespace hazard {

class Collector {
  friend class Mutator;

 public:
  static Collector& Instance();

  ~Collector();

  Mutator* GetMutator();

 private:
  std::atomic<Mutator*> head_{nullptr};
};

}  // namespace hazard
