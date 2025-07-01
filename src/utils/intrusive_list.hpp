#pragma once

#include <cassert>
#include <cstddef>

namespace utils {

// ANCHOR : Represents node of circular doubly-linked list.

template <typename T>
struct IntrusiveListNode {
  using Node = IntrusiveListNode;

  Node* prev = nullptr;
  Node* next = nullptr;

  Node* Prev() noexcept { return prev; }

  Node* Next() noexcept { return next; }

  // Links this node before next in list.

  // Precondition: !IsLinked().
  void LinkBefore(Node* that) noexcept {
    assert(!IsLinked());

    prev = that->prev;
    prev->next = this;
    next = that;
    that->prev = this;
  }

  bool IsLinked() const noexcept { return next != nullptr; }

  // Precondition: IsLinked().
  void Unlink() noexcept {
    assert(IsLinked());

    next->prev = prev;
    prev->next = next;

    next = prev = nullptr;
  }

  T* AsItem() noexcept { return static_cast<T*>(this); }
};

// ANCHOR : Circular doubly-linked intrusive list.

template <typename T>
class IntrusiveList {
  using Node = IntrusiveListNode<T>;
  using List = IntrusiveList<T>;

 public:
  IntrusiveList() { InitEmpty(); }

  // Intentionally disabled.
  IntrusiveList& operator=(List&& that) = delete;

  IntrusiveList(List&& that) {
    InitEmpty();
    Append(that);
  }

  // Non-copyable.
  IntrusiveList(const List& that) = delete;
  IntrusiveList& operator=(const List& that) = delete;

  ~IntrusiveList() { assert(IsEmpty()); }

  void PushBack(Node* node) noexcept { node->LinkBefore(&enter_); }

  void PushFront(Node* node) noexcept { node->LinkBefore(FrontNode()); }

  // Precondition: NonEmpty().
  T* PopFrontNonEmpty() noexcept {
    assert(NonEmpty());

    Node* front = FrontNode();
    front->Unlink();
    return front->AsItem();
  }

  // Returns nullptr if empty.
  T* TryPopFront() noexcept {
    if (IsEmpty()) {
      return nullptr;
    }
    return PopFrontNonEmpty();
  }

  [[deprecated("Use TryPopFront")]]
  T* PopFront() noexcept {
    return TryPopFront();
  }

  // Precondition: NonEmpty().
  T* PopBackNonEmpty() noexcept {
    assert(NonEmpty());

    Node* back = BackNode();
    back->Unlink();
    return back->AsItem();
  }

  // Returns nullptr if empty.
  T* TryPopBack() noexcept {
    if (IsEmpty()) {
      return nullptr;
    }
    return PopBackNonEmpty();
  }

  [[deprecated("Use TryPopBack")]]
  T* PopBack() noexcept {
    return TryPopBack();
  }

  // ANCHOR : IsEmpty.

  bool IsEmpty() const noexcept { return enter_.next == &enter_; }

  bool NonEmpty() const noexcept { return !IsEmpty(); }

  bool HasItems() const noexcept { return !IsEmpty(); }

  // ANCHOR :FrontNonEmpty.
  // Precondition: NonEmpty().

  T* FrontNonEmpty() noexcept {
    assert(NonEmpty());
    return FrontNode()->AsItem();
  }

  const T* FrontNonEmpty() const noexcept {
    assert(NonEmpty());
    return FrontNode()->AsItem();
  }

  // ANCHOR : Front.

  T* TryFront() noexcept {
    if (IsEmpty()) {
      return nullptr;
    }
    return FrontNonEmpty();
  }

  [[deprecated("Use TryFront")]]
  T* Front() noexcept {
    return TryFront();
  }

  const T* TryFront() const noexcept {
    if (IsEmpty()) {
      return nullptr;
    }
    return FrontNonEmpty();
  }

  [[deprecated("Use TryFront")]]
  const T* Front() const noexcept {
    return TryFront();
  }

  // ANCHOR : BackNonEmpty.
  // Precondition: NonEmpty().

  T* BackNonEmpty() noexcept {
    assert(NonEmpty());
    return BackNode()->AsItem();
  }

  const T* BackNonEmpty() const noexcept {
    assert(NonEmpty());
    return BackNode()->AsItem();
  }

  // ANCHOR : Back.

  T* TryBack() noexcept {
    if (IsEmpty()) {
      return nullptr;
    }
    return BackNonEmpty();
  }

  [[deprecated("Use TryBack")]]
  T* Back() noexcept {
    return TryBack();
  }

  const T* TryBack() const noexcept {
    if (IsEmpty()) {
      return nullptr;
    }
    return BackNonEmpty();
  }

  [[deprecated("Use TryBack")]]
  const T* Back() const noexcept {
    return TryBack();
  }

  // Append (= move, re-link) all nodes from `that` list to the end of this
  // list. Post-condition: that.IsEmpty() == true.
  void Append(List& that) noexcept {
    if (that.IsEmpty()) {
      return;
    }

    Node* that_front = that.enter_.next;
    Node* that_back = that.enter_.prev;

    that_back->next = &enter_;
    that_front->prev = enter_.prev;

    Node* back = enter_.prev;

    enter_.prev = that_back;
    back->next = that_front;

    that.enter_.next = that.enter_.prev = &that.enter_;
  }

  // Complexity: O(1).
  void Swap(List& that) {
    List tmp;
    tmp.Append(*this);
    Append(that);
    that.Append(tmp);
  }

 private:
  void InitEmpty() { enter_.next = enter_.prev = &enter_; }

  Node* FrontNode() noexcept { return enter_.next; }

  const Node* FrontNode() const noexcept { return enter_.next; }

  Node* BackNode() noexcept { return enter_.prev; }

  const Node* BackNode() const noexcept { return enter_.prev; }

 private:
  Node enter_;  // Sentinel node.
};

}  // namespace utils
