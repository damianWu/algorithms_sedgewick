// Copyright [2024] <@damianWu>

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include "ch1/ch1.hpp"

namespace ch1
{
namespace queue
{
TEST(QueueTest, addNewElementToQueue)
{
  constexpr auto expectedSize{1};
  Queue<std::string> queue;
  std::string item{"First item"};

  queue.enqueue(item);

  ASSERT_EQ(expectedSize, queue.size());
  ASSERT_FALSE(queue.isEmpty());
}

TEST(QueueTest, addNewElementsToQueue)
{
  constexpr auto expectedSize{4};
  Queue<std::string> queue;
  std::string item1{"First item"};
  std::string item2{"2 item"};
  std::string item3{"3 item"};
  std::string item4{"4 item"};

  queue.enqueue(item1);
  queue.enqueue(item2);
  queue.enqueue(item3);
  queue.enqueue(item4);

  ASSERT_EQ(expectedSize, queue.size());
  ASSERT_FALSE(queue.isEmpty());
}

TEST(QueueTest, removeElementFromQueue)
{
  constexpr auto expectedSize{0};
  constexpr auto expectedSizeAfterPush{1};
  Queue<std::string> queue;
  std::string expectedItem{"First item"};

  queue.enqueue(expectedItem);

  ASSERT_EQ(expectedSizeAfterPush, queue.size());

  const std::string item{queue.dequeue()};
  const std::string emptyItem{queue.dequeue()};
  queue.dequeue();
  queue.dequeue();

  ASSERT_EQ("", emptyItem);
  ASSERT_EQ(expectedItem, item);
  ASSERT_EQ(expectedSize, queue.size());
  ASSERT_TRUE(queue.isEmpty());
}

TEST(QueueTest, removeElementsFromQueue)
{
  constexpr auto expectedFinalSize{0};
  constexpr auto expectedSizeAfterPush{4};
  Queue<std::string> queue;
  std::string item1{"First item"};
  std::string item2{"2nd item"};
  std::string item3{"3rd item"};
  std::string item4{"4th item"};

  queue.enqueue(item1);
  queue.enqueue(item2);
  queue.enqueue(item3);
  queue.enqueue(item4);

  ASSERT_EQ(expectedSizeAfterPush, queue.size());

  const std::string returnedItem1{queue.dequeue()};
  const std::string returnedItem2{queue.dequeue()};
  const std::string returnedItem3{queue.dequeue()};
  const std::string returnedItem4{queue.dequeue()};

  ASSERT_EQ(item1, returnedItem1);
  ASSERT_EQ(item2, returnedItem2);
  ASSERT_EQ(item3, returnedItem3);
  ASSERT_EQ(item4, returnedItem4);
  ASSERT_EQ(expectedFinalSize, queue.size());
  ASSERT_TRUE(queue.isEmpty());
}

TEST(QueueTest, clearShouldRemoveAllElements)
{
  constexpr auto expectedFinalSize{0};
  constexpr auto expectedSizeAfterPush{5};
  Queue<std::string> queue;

  queue.enqueue("item1");
  queue.enqueue("item2");
  queue.enqueue("item3");
  queue.enqueue("item4");
  queue.enqueue("item5");

  ASSERT_EQ(expectedSizeAfterPush, queue.size());

  queue.clear();

  ASSERT_EQ(expectedFinalSize, queue.size());
  ASSERT_TRUE(queue.isEmpty());
}

TEST(QueueTest, iterateThroughList)
{
  std::vector<std::string> items{"item1", "item2", "item3", "item4", "item5"};

  Queue<std::string> queue;
  for (const auto& s : items)
  {
    queue.enqueue(s);
  }

  std::size_t i{};
  for (const auto& e : queue)
  {
    if (e.item != items.at(i))
    {
      FAIL() << "Every corresponding element should be equal.";
    }
    ++i;
  }
}

TEST(QueueTest, removeKthElement)
{
  constexpr auto expectedSize{4};
  const std::string_view expectedItem{"item4"};

  Queue<std::string> queue;
  queue.enqueue("item1");
  queue.enqueue("item2");
  queue.enqueue("item3");  // 2
  queue.enqueue("item4");
  queue.enqueue("item5");

  queue.remove(2);
  ASSERT_EQ(expectedSize, queue.size());

  queue.dequeue();
  queue.dequeue();
  ASSERT_EQ(expectedItem, queue.dequeue());
}

TEST(QueueTest, removeOutOfBounds)
{
  constexpr auto expectedResult{false};

  Queue<std::string> queue;
  queue.enqueue("item1");
  queue.enqueue("item2");

  ASSERT_EQ(expectedResult, queue.remove(20));
}

TEST(QueueTest, removeWithSizeOne)
{
  constexpr auto expectedResult{true};

  Queue<std::string> queue;
  queue.enqueue("item1");

  ASSERT_EQ(expectedResult, queue.remove(0));
}

}  // namespace queue

namespace efficient_stack
{
using CapacityType = std::size_t;
using SizeType = std::size_t;
using ElementType = int32_t;

class AddNewElementsToStackTest
    : public ::testing::TestWithParam<std::tuple<CapacityType, SizeType, std::vector<ElementType>>>
{
};

TEST_P(AddNewElementsToStackTest, shouldAddNewElementsToStack)
{
  const auto& [capacity, expectedSize, elements]{GetParam()};

  Stack<int32_t> stack{capacity};

  std::ranges::for_each(elements, [&stack](auto e) { stack.push(e); });

  ASSERT_EQ(expectedSize, stack.size());
}

INSTANTIATE_TEST_SUITE_P(NewStackElementsTest, AddNewElementsToStackTest,
                         testing::Values(std::make_tuple(2, 2, std::vector<ElementType>{99, 980}),
                                         std::make_tuple(0, 4,
                                                         std::vector<ElementType>{99, 980, 980, 980})));

TEST(StackTest, shouldSizeBeCalculatedCorrectly)
{
  Stack<int32_t> stack{2};
  constexpr auto expectedSize{0};

  ASSERT_EQ(expectedSize, stack.size());
}

TEST(StackTest, isEmptyWorksCorrectly)
{
  Stack<int32_t> stack;
  constexpr auto expectedResult{true};

  ASSERT_EQ(expectedResult, stack.isEmpty());
}

TEST(StackTest, rbeginShouldReturnLastElement)
{
  Stack<int32_t> stack{20};

  stack.push(1);
  stack.push(2);
  stack.push(374563123);

  ASSERT_EQ(*(stack.end() - 1), *stack.rbegin());
}

TEST(StackTest, rendShouldReturnFirstElement)
{
  Stack<int32_t> stack{10};

  stack.push(834278671);
  stack.push(2);
  stack.push(3);

  ASSERT_EQ(*(stack.begin()), *(stack.rend() - 1));
}

TEST(StackTest, stackShouldPopElements)
{
  Stack<int32_t> stack{10};

  constexpr size_t expectedSize{2};
  constexpr size_t expectedElement{3};

  stack.push(1);
  stack.push(2);
  stack.push(3);

  ASSERT_EQ(expectedElement, stack.pop());
  ASSERT_EQ(expectedSize, stack.size());
}

TEST(StackTest, stackShouldPopAllElementsAndHaveToBeEmpty)
{
  Stack<int32_t> stack{1};

  constexpr size_t expectedSize{0};
  constexpr size_t expectedElement1{3};
  constexpr size_t expectedElement2{2};
  constexpr size_t expectedElement3{1};

  stack.push(1);
  stack.push(2);
  stack.push(3);

  ASSERT_EQ(expectedElement1, stack.pop());
  ASSERT_EQ(expectedElement2, stack.pop());
  ASSERT_EQ(expectedElement3, stack.pop());
  ASSERT_EQ(expectedSize, stack.size());
}

TEST(StackTest, stackShouldDoNothingWhenPopIfStackIsEmpty)
{
  constexpr size_t expectedSize{0};
  Stack<int32_t> stack{0};

  stack.pop();
  stack.pop();
  stack.pop();
  stack.pop();

  ASSERT_EQ(expectedSize, stack.size());
}

TEST(StackTest, stackShouldDoNothingWhenPopIfStackIsEmpty2)
{
  constexpr size_t expectedSize{0};
  Stack<int32_t> stack{0};

  stack.push(4);
  stack.push(5);
  stack.push(6);

  stack.pop();
  stack.pop();
  stack.pop();
  stack.pop();
  stack.pop();
  stack.pop();

  ASSERT_EQ(expectedSize, stack.size());
}
TEST(StackTest, shouldReturnValueWithoutModifyingStack)
{
  constexpr std::string_view expectedItem{"item4"};
  constexpr auto expectedSize{4};

  Stack<std::string> stack;
  stack.push("item1");
  stack.push("item2");
  stack.push("item3");
  stack.push("item4");

  ASSERT_EQ(expectedItem, stack.peek());
  ASSERT_EQ(expectedSize, stack.size());
}

}  // namespace efficient_stack

namespace linked_list_stack
{

struct LinkedListStackTest : public ::testing::Test
{
  void TearDown() override { stack.clear(); }

  Stack<std::string> stack{};  // NOLINT (misc-non-private-member-variables-in-classes)
};

TEST_F(LinkedListStackTest, newlyCreatedStackShouldBeEmpty)
{
  constexpr std::size_t expectedSize{0};

  ASSERT_EQ(expectedSize, stack.size());
  ASSERT_TRUE(stack.isEmpty());
}

TEST_F(LinkedListStackTest, clearShouldRemoveAllElementsFromStack)
{
  constexpr std::size_t expectedSize{0};

  stack.push("1");
  stack.push("2");
  stack.push("3");

  stack.clear();

  ASSERT_EQ(expectedSize, stack.size());
  ASSERT_TRUE(stack.isEmpty());
}

TEST_F(LinkedListStackTest, pushItemToStack)
{
  constexpr std::size_t expectedSize{1};

  stack.push(std::string{"push_test"});

  ASSERT_EQ(expectedSize, stack.size());
  ASSERT_FALSE(stack.isEmpty());
}

TEST_F(LinkedListStackTest, pushItemsToStack)
{
  constexpr std::size_t expectedSize{4};

  stack.push(std::string{"push_test1"});
  stack.push(std::string{"push_test2"});
  stack.push(std::string{"push_test3"});
  stack.push(std::string{"push_test4"});

  ASSERT_EQ(expectedSize, stack.size());
  ASSERT_FALSE(stack.isEmpty());
}

TEST_F(LinkedListStackTest, popItemFromStack)
{
  constexpr std::size_t expectedFinalSize{0};
  constexpr std::size_t expectedAfterPushSize{1};
  const std::string expectedPopResult{"push_test1"};

  stack.push(expectedPopResult);

  ASSERT_EQ(expectedAfterPushSize, stack.size());

  const auto popResult{stack.pop()};
  stack.pop();
  stack.pop();
  stack.pop();
  stack.pop();

  ASSERT_EQ(expectedPopResult, popResult);
  ASSERT_EQ(expectedFinalSize, stack.size());
  ASSERT_TRUE(stack.isEmpty());
}

TEST_F(LinkedListStackTest, popItemsFromStack)
{
  constexpr std::size_t expectedFinalSize{0};
  constexpr std::size_t expectedAftefPushSize{4};
  const std::string expectedPopResult1{"push_test1"};
  const std::string expectedPopResult2{"push_test2"};
  const std::string expectedPopResult3{"push_test3"};
  const std::string expectedPopResult4{"push_test4"};

  stack.push(expectedPopResult1);
  stack.push(expectedPopResult2);
  stack.push(expectedPopResult3);
  stack.push(expectedPopResult4);

  ASSERT_EQ(expectedAftefPushSize, stack.size());

  const auto popResult4{stack.pop()};
  const auto popResult3{stack.pop()};
  const auto popResult2{stack.pop()};
  const auto popResult1{stack.pop()};

  ASSERT_EQ(expectedPopResult1, popResult1);
  ASSERT_EQ(expectedPopResult2, popResult2);
  ASSERT_EQ(expectedPopResult3, popResult3);
  ASSERT_EQ(expectedPopResult4, popResult4);
  ASSERT_EQ(expectedFinalSize, stack.size());
  ASSERT_TRUE(stack.isEmpty());
}

TEST_F(LinkedListStackTest, iterateThroughStack)
{
  std::vector<std::string> items{"item1", "item2", "item3", "item4", "item5"};

  for (const auto& s : items)
  {
    stack.push(s);
  }

  std::size_t i{items.size() - 1};
  for (const auto& e : stack)
  {
    if (e.item != items.at(i))
    {
      FAIL() << "Every corresponding element should be equal.";
    }
    --i;
  }
}
}  // namespace linked_list_stack

namespace homework
{

class Ex1_3_5Test : public ::testing::TestWithParam<std::pair<std::string, bool>>
{
};

TEST_P(Ex1_3_5Test, parenthesisTest)
{
  const auto& [input, expectedResult]{GetParam()};
  const auto result{::homework::ex1_3_5(input)};
  ASSERT_EQ(expectedResult, result);
}

INSTANTIATE_TEST_SUITE_P(DISABLED_Ex1_3_5Test, Ex1_3_5Test,
                         testing::Values(std::make_pair("[()]{}{[()()]()}", true),
                                         std::make_pair("[(])", false), std::make_pair("[(", false),
                                         std::make_pair("", true), std::make_pair("[[]]", true),
                                         std::make_pair("[([])]{}{", false),
                                         std::make_pair("asddas", false)));

}  // namespace homework

}  // namespace ch1
