// Copyright [2024] <@damianWu>

#include "ch1/ch1.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>

namespace ch1
{
namespace homework
{
bool ex1_3_5(std::string_view input)
{
  ch1::efficient_stack::Stack<char> opening_parenthesis;

  const std::string open{"[({"};
  const std::string close{"])}"};

  for (const auto c : input)
  {
    if (open.contains(c))
    {
      opening_parenthesis.push(c);
    }
    else if (close.contains(c))
    {
      // c is closing bracket
      // o is opening bracket
      const auto o{opening_parenthesis.pop()};
      if (open.find(o) != close.find(c))
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
  return opening_parenthesis.isEmpty();
}

void ex1_3_37(int32_t n, int32_t m)
{
  if (n < m || m == 0)
  {
    return;
  }

  ch1::queue::QueueImpl<int32_t> queue;

  for (int32_t i{0}; i < n; ++i)
  {
    queue.enqueue(i);
  }

  int32_t indexToDel{m - 1};
  while (queue.size() != 1)
  {
    for (size_t i{}; i < queue.size() / m + 1; ++i)
    {
      const auto re{queue.remove(indexToDel)};
      fmt::print("{} ", re.value_or(-1));
      indexToDel = (indexToDel + m - 1) % static_cast<int32_t>(queue.size());
    }
  }
  const auto re{queue.remove(indexToDel)};
  fmt::print("{}\n", re.value_or(-1));
}

// Move to front
std::string ex1_3_40(const std::string& input)
{
  double_linked_list::DoubleLinkedList<char> list;

  for (const auto c : input)
  {
    if (list.find(c) != std::nullopt)
    {
      list.remove(c);
      list.pushRight(c);
      continue;
    }
    list.pushRight(c);
  }

  std::string output;
  std::transform(std::begin(list), std::end(list), std::back_inserter(output),
                 [](const auto& node) { return node.item; });
  return output;
}
}  // namespace homework
}  // namespace ch1