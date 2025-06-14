#include <lf/lock_free_queue.hpp>

#include <gtest/gtest.h>
#include <string>

using lf::LockFreeQueue;

TEST(LockFreeQueueUnitTest, JustWorks) {
  LockFreeQueue<std::string> queue;

  queue.Push("Payload");
  auto v = queue.TryPop();
  ASSERT_TRUE(v);
  EXPECT_EQ(*v, "Payload");

  auto empty = queue.TryPop();
  EXPECT_FALSE(empty);
}

TEST(LockFreeQueueUnitTest, FIFO) {
  LockFreeQueue<int> queue;

  queue.Push(1);
  queue.Push(2);
  queue.Push(3);

  {
    auto v = queue.TryPop();
    ASSERT_TRUE(v);
    EXPECT_EQ(*v, 1);
  }

  {
    auto v = queue.TryPop();
    ASSERT_TRUE(v);
    EXPECT_EQ(*v, 2);
  }

  {
    auto v = queue.TryPop();
    ASSERT_TRUE(v);
    EXPECT_EQ(*v, 3);
  }

  EXPECT_FALSE(queue.TryPop());
}

TEST(LockFreeQueueUnitTest, Dtor) {
  {
    LockFreeQueue<std::string> queue;
    queue.Push("Hello");
    queue.Push("World");
  }
}

struct MoveOnly {
  MoveOnly() = default;
  MoveOnly(MoveOnly&&) = default;
  MoveOnly& operator=(MoveOnly&&) = default;

  MoveOnly(const MoveOnly&) = delete;
  MoveOnly& operator=(const MoveOnly&) = delete;
};

TEST(LockFreeQueueUnitTest, MoveOnlyValue) {
  LockFreeQueue<MoveOnly> queue;
  queue.Push(MoveOnly{});
  EXPECT_TRUE(queue.TryPop());
}

struct NonDefaultConstructible {
  explicit NonDefaultConstructible(int) {}
};

TEST(LockFreeQueueUnitTest, NonDefaultConstructibleValue) {
  LockFreeQueue<NonDefaultConstructible> queue;
  queue.Push(NonDefaultConstructible{7});
  EXPECT_TRUE(queue.TryPop());
}

TEST(LockFreeQueueUnitTest, TwoQueuesIndependent) {
  LockFreeQueue<int> queue_1;
  LockFreeQueue<int> queue_2;

  queue_1.Push(3);
  queue_2.Push(11);

  auto v1 = queue_1.TryPop();
  ASSERT_TRUE(v1);
  EXPECT_EQ(*v1, 3);

  auto v2 = queue_2.TryPop();
  ASSERT_TRUE(v2);
  EXPECT_EQ(*v2, 11);
}
