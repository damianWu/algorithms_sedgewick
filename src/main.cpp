// Copyright [2024] <@damianWu>
#include <fmt/core.h>

#include <iostream>
#include <memory>

int main()
{
   try
   {
      fmt::print("I'd rather be {1} than {0}.\n", "right", "happy");
   }
   catch (const std::exception &e)
   {
      std::cerr << "Exception catch in main function with message: " << e.what()
                << '\n';
   }
   catch (...)
   {
      std::cerr << "Unknown type of exception catch in main function" << '\n';
   }
   return 0;
}
