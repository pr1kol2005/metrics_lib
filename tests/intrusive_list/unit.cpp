#include <utils/intrusive_list.hpp>

#include <gtest/gtest.h>

using utils::IntrusiveList;
using utils::IntrusiveListNode;

struct Item : IntrusiveListNode<Item> {
  int value;

  explicit Item(int v) : value(v) {}
};

TEST(IntrusiveListUnitTest, IsInitiallyEmpty) {
  IntrusiveList<Item> list;
  EXPECT_TRUE(list.IsEmpty());
  EXPECT_FALSE(list.NonEmpty());
}

TEST(IntrusiveListUnitTest, PushBackAndPopFront) {
  IntrusiveList<Item> list;
  Item a(1), b(2);
  list.PushBack(&a);
  list.PushBack(&b);

  EXPECT_FALSE(list.IsEmpty());
  EXPECT_EQ(list.FrontNonEmpty()->value, 1);
  EXPECT_EQ(list.BackNonEmpty()->value, 2);

  EXPECT_EQ(list.PopFrontNonEmpty()->value, 1);
  EXPECT_EQ(list.PopFrontNonEmpty()->value, 2);
  EXPECT_TRUE(list.IsEmpty());
}

TEST(IntrusiveListUnitTest, PushFrontAndPopBack) {
  IntrusiveList<Item> list;
  Item a(1), b(2);
  list.PushFront(&a);
  list.PushFront(&b);

  EXPECT_EQ(list.TryBack()->value, 1);
  EXPECT_EQ(list.PopBackNonEmpty()->value, 1);
  EXPECT_EQ(list.PopBackNonEmpty()->value, 2);
  EXPECT_TRUE(list.IsEmpty());
}

TEST(IntrusiveListUnitTest, TryPopOnEmpty) {
  IntrusiveList<Item> list;
  EXPECT_EQ(list.TryPopFront(), nullptr);
  EXPECT_EQ(list.TryPopBack(), nullptr);
  EXPECT_EQ(list.TryFront(), nullptr);
  EXPECT_EQ(list.TryBack(), nullptr);
}

TEST(IntrusiveListUnitTest, SwapLists) {
  IntrusiveList<Item> a, b;
  Item i1(1), i2(2);
  a.PushBack(&i1);
  b.PushBack(&i2);

  a.Swap(b);

  EXPECT_EQ(b.TryFront()->value, 1);
  EXPECT_EQ(a.TryFront()->value, 2);

  while (a.TryPopBack()) {
  }
  while (b.TryPopBack()) {
  }
}
