#pragma once

#include <utils/intrusive_list.hpp>

#include <concepts>

namespace hazard {

struct ManagedBase : utils::IntrusiveListNode<ManagedBase> {
  void virtual Destroy() = 0;

  virtual ~ManagedBase() = default;
};

template <typename T>
struct Managed : ManagedBase {
  void Destroy() final { delete static_cast<T*>(this); }
};

template <typename T>
concept IsManaged = std::derived_from<T, Managed<T>>;

}  // namespace hazard
