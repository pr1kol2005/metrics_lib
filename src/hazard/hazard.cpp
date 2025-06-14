#include <unordered_set>
#include <vector>

#include "collector.hpp"

namespace hazard {

//////////////////////////////////////////////////////////////////////

PtrResetGuard::~PtrResetGuard() { owner_.Reset(); }

//////////////////////////////////////////////////////////////////////

static thread_local Mutator* local_mutator = nullptr;

Collector& Collector::Instance() {
  static Collector instance;
  return instance;
}

Collector::~Collector() {
  std::vector<Mutator*> to_delete;

  Mutator* head = head_.exchange(nullptr, std::memory_order_acquire);

  while (head != nullptr) {
    Mutator* next = head->next_.load();
    to_delete.push_back(head);
    head = next;
  }

  for (Mutator* ptr : to_delete) {
    delete ptr;
  }
}

Mutator* Collector::GetMutator() {
  if (local_mutator == nullptr) {
    local_mutator = new Mutator(this);

    Mutator* expected = head_.load();
    do {
      local_mutator->next_.store(expected);
    } while (!head_.compare_exchange_weak(expected, local_mutator,
                                          std::memory_order_release));
  }
  return local_mutator;
}

//////////////////////////////////////////////////////////////////////

Mutator* GetMutator() { return Collector::Instance().GetMutator(); }

//////////////////////////////////////////////////////////////////////

// ANCHOR : Garbage collection logic

void Mutator::Gc() {
  // Collect all protected pointers
  std::unordered_set<void*> protected_set;
  for (Mutator* m = collector_->head_.load(std::memory_order_acquire);
       m != nullptr; m = m->next_.load(std::memory_order_acquire)) {
    for (auto& hp : m->hazard_ptrs_) {
      if (void* p = hp.Get()) {
        protected_set.insert(p);
      }
    }
  }

  // Filter and delete retired objects
  utils::IntrusiveList<ManagedBase> tmp;
  retired_.Swap(tmp);

  while (auto it = tmp.TryPopFront()) {
    if (protected_set.contains(it)) {
      retired_.PushBack(it);
    } else {
      it->Destroy();
      --retired_size_;
    }
  }
}

}  // namespace hazard
