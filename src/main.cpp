// Copyright [2024] <@damianWu>
#include <fmt/core.h>

#include <iostream>

#include "ch1/ch1.hpp"

int main()
{
   try
   {
   std::vector<int> v{5634,12};
   std::cout << v.size() << '\n';
   std::ranges::for_each(v,[](auto e){
      std::cout << e << '\n';
   });
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
