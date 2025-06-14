#pragma once

#include <atomic>
#include <optional>

#include <hazard/user.hpp>

namespace lf {

// Treiber unbounded MPMC lock-free stack

template <typename T>
class LockFreeStack {
  struct Node : hazard::Managed<Node> {
    T value;
    Node* next{nullptr};

    explicit Node(T v) : value(std::move(v)) {}
  };

 public:
  LockFreeStack() = default;

  // Push to top
  void Push(T value) {
    Node* new_node = new Node(std::move(value));
    new_node->next = top_.load();

    while (!top_.compare_exchange_weak(new_node->next, new_node)) {}
  }

  // Try pop from top
  std::optional<T> TryPop() {
    auto* mutator = hazard::GetMutator();

    while (true) {
      auto top_guard = mutator->GetHazardPtr(0);

      Node* curr_top = top_guard.Protect(top_);

      if (curr_top == nullptr) {
        return std::nullopt;
      }

      if (top_.compare_exchange_weak(curr_top, curr_top->next)) {
        T value = std::move(curr_top->value);
        mutator->Retire(curr_top);
        return value;
      }
    }
  }

  ~LockFreeStack() {
    Node* curr = top_.load();
    while (curr != nullptr) {
      Node* next = curr->next;
      delete curr;
      curr = next;
    }
  }

 private:
  std::atomic<Node*> top_{nullptr};
};

}  // namespace lf
