#pragma once

#include <hazard/user.hpp>

#include <atomic>
#include <optional>

namespace lf {

// Michael-Scott unbounded lock-free queue.

template <typename T>
class LockFreeQueue {
  struct Node : hazard::Managed<Node> {
    std::optional<T> value;
    std::atomic<Node*> next{nullptr};

    Node() {}

    explicit Node(T v) : value(std::move(v)) {}
  };

 public:
  LockFreeQueue() {
    Node* dummy = new Node{};

    head_.store(dummy);
    tail_.store(dummy);
  }

  // Push to tail.
  void Push(T value) {
    Node* new_tail = new Node(std::move(value));

    auto mutator = hazard::GetMutator();
    auto tail_guard = mutator->GetHazardPtr(0);

    while (true) {
      auto tail = tail_guard.Protect(tail_);

      auto next = tail->next.load();

      if (tail != tail_.load()) {
        continue;
      }

      if (next != nullptr) {
        tail_.compare_exchange_weak(tail, next);
        continue;
      }

      Node* temp = nullptr;
      if (tail->next.compare_exchange_strong(temp, new_tail)) {
        tail_.compare_exchange_strong(tail, new_tail);
        break;
      }
    }
  }

  // Try pop from head.
  std::optional<T> TryPop() {
    auto mutator = hazard::GetMutator();

    auto head_guard = mutator->GetHazardPtr(0);
    auto next_guard = mutator->GetHazardPtr(1);

    while (true) {
      auto head = head_guard.Protect(head_);

      if (head_.load() != head) {
        continue;
      }

      auto next = next_guard.Protect(head->next);

      if (next == nullptr) {
        return std::nullopt;
      }

      auto tail = tail_.load();
      if (head == tail) {
        tail_.compare_exchange_strong(tail, next);
        continue;
      }

      if (head_.compare_exchange_strong(head, next)) {
        std::optional<T> result = std::move(next->value);
        mutator->Retire(head);
        return result;
      }
    }
  }

  ~LockFreeQueue() {
    while (head_ != nullptr) {
      Node* to_remove = head_.load();
      head_ = to_remove->next.load();
      delete to_remove;
    }
  }

 private:
  std::atomic<Node*> head_{nullptr};
  std::atomic<Node*> tail_{nullptr};
};

}  // namespace lf
