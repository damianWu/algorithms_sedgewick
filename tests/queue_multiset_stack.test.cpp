#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <vector>

#include "ch1/ch1.hpp"

namespace ch1
{

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

INSTANTIATE_TEST_SUITE_P(
    NewStackElementsTest, AddNewElementsToStackTest,
    testing::Values(
        std::make_tuple<CapacityType, SizeType, std::vector<ElementType>>(2, 2, {99, 980}),
        std::make_tuple<CapacityType, SizeType, std::vector<ElementType>>(0, 4, {99, 980, 980, 980})));

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
}  // namespace efficient_stack

}  // namespace ch1
