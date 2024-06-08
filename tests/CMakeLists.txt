# Set TARGET(binary file) name for convenience
set(queue_multiset_stack_test ${PROJECT_NAME}_queue_multiset_stack_test)

# Test files
set(
        TEST_FILES
        queue_multiset_stack.test.cpp
)

# (Names of) Tested libraries
set(
        TESTED_LIBRARIES
        ch1_lib
)

# Executable that runs the tests.
add_executable(
        ${queue_multiset_stack_test}             # Executable tests file name
        main.test.cpp
        ${TEST_FILES}
)

# Link the executable to needed libraries.
target_link_libraries(
        ${queue_multiset_stack_test}
        ${TESTED_LIBRARIES}     # Library we are testing
        GTest::gtest_main
        gtest::gtest        # GTest libraries
)

set(gtest_filter "--gtest_filter=*LinkedListTest*")

# Add gtest to be able to run ctest.
add_test(
        NAME queueMultiSetStack            # Test name
        COMMAND ${queue_multiset_stack_test} --gtest_color=1 ${gtest_filter}   # Executable tests file name
)

# include(GoogleTest)
# gtest_discover_tests(${queue_multiset_stack_test})
