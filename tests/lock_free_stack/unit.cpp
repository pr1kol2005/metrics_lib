#include <lf/lock_free_stack.hpp>

#include <gtest/gtest.h>

TEST(LockFreeStackUnit, JustWorks) {
  lf::LockFreeStack<std::string> stack;

  stack.Push("Data");
  auto v = stack.TryPop();
  EXPECT_TRUE(v.has_value());
  if (v) {
    EXPECT_EQ(*v, "Data");
  }

  auto empty = stack.TryPop();
  EXPECT_FALSE(empty.has_value());
}

TEST(LockFreeStackUnit, LIFO) {
  lf::LockFreeStack<int> stack;

  stack.Push(1);
  stack.Push(2);
  stack.Push(3);

  auto v = stack.TryPop();
  EXPECT_TRUE(v.has_value());
  if (v) {
    EXPECT_EQ(*v, 3);
  }

  v = stack.TryPop();
  EXPECT_TRUE(v.has_value());
  if (v) {
    EXPECT_EQ(*v, 2);
  }

  v = stack.TryPop();
  EXPECT_TRUE(v.has_value());
  if (v) {
    EXPECT_EQ(*v, 1);
  }

  EXPECT_FALSE(stack.TryPop().has_value());
}

TEST(LockFreeStackUnit, Destructor) {
  {
    lf::LockFreeStack<std::string> stack;
    stack.Push("Hello");
    stack.Push("World");
  }
  // Проверяется, что при выходе из блока деструктор не падает (нет явных
  // ASSERT)
}

struct MoveOnly {
  MoveOnly() = default;

  MoveOnly(MoveOnly&&) = default;
  MoveOnly& operator=(MoveOnly&&) = default;

  MoveOnly(const MoveOnly&) = delete;
  MoveOnly& operator=(const MoveOnly&) = delete;
};

TEST(LockFreeStackUnit, MoveOnlyValue) {
  lf::LockFreeStack<MoveOnly> stack;
  stack.Push(MoveOnly{});
  EXPECT_TRUE(stack.TryPop().has_value());
}

struct NonDefaultConstructible {
  explicit NonDefaultConstructible(int) {}
};

TEST(LockFreeStackUnit, NonDefaultConstructibleValue) {
  lf::LockFreeStack<NonDefaultConstructible> stack;
  stack.Push(NonDefaultConstructible{7});
  EXPECT_TRUE(stack.TryPop().has_value());
}

TEST(LockFreeStackUnit, TwoStacks) {
  lf::LockFreeStack<int> stack_1;
  lf::LockFreeStack<int> stack_2;

  stack_1.Push(3);
  stack_2.Push(11);

  auto v = stack_1.TryPop();
  EXPECT_TRUE(v.has_value());
  if (v) {
    EXPECT_EQ(*v, 3);
  }

  v = stack_2.TryPop();
  EXPECT_TRUE(v.has_value());
  if (v) {
    EXPECT_EQ(*v, 11);
  }
}
