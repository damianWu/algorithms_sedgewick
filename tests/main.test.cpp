// "Copyright [2024] <@damianWu>"

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   // ::testing::GTEST_FLAG(filter) = "*shouldPutNewItemBeforeAnotherOne*";
   return RUN_ALL_TESTS();
}
