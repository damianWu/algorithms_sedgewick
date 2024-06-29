// Copyright [2024] <@damianWu>

#include "ch1/ch1.hpp"

#include <string>

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

}  // namespace homework