// Copyright [2024] <@damianWu>
#include <fmt/core.h>

#include <cstdio>
#include <iostream>
#include "ch1/ch1.hpp"

int main()
{
  try
  {
    // ch1::queue::Queue<std::string> strq;
    // strq.enqueue("item1");
    // strq.enqueue("item2");
    // strq.enqueue("item3");
    // strq.enqueue("item4");

    // strq.remove(2);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Exception catch in main function with message: " << e.what() << '\n';
  }
  catch (...)
  {
    std::cerr << "Unknown type of exception catch in main function" << '\n';
  }
  return 0;
}
