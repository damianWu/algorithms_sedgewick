// Copyright [2024] <@damianWu>
#include <fmt/core.h>

int main()
{
  try
  {
  }
  catch (const std::exception& e)
  {
    fmt::println("Exception catch in main function with message: {}", e.what());
  }
  catch (...)
  {
    fmt::println("Unknown type of exception catch in main function");
  }
  return 0;
}
