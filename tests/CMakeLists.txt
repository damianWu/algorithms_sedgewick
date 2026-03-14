# Set TARGET(binary file) name for convenience
set(ch1_test ${PROJECT_NAME}_ch1_test)

# Test files
set(
        TEST_FILES
        main.test.cpp
        ch1.test.cpp
)

# (Names of) Tested libraries
set(
        TESTED_LIBRARIES
        ch1_lib
)

# Executable that runs the tests.
add_executable(
        ${ch1_test}             # Executable tests file name
        main.test.cpp
        ${TEST_FILES}
)

# Link the executable to needed libraries.
target_link_libraries(
        ${ch1_test}
        ${TESTED_LIBRARIES}     # Library we are testing
        GTest::gtest_main
        gtest::gtest        # GTest libraries
)

# set(gtest_filter "--gtest_filter=*ex1_3_37Test2*")
set(gtest_filter "--gtest_filter=*binarySearchTest*")

# Add gtest to be able to run ctest.
add_test(
        NAME queueMultiSetStack            # Test name
        COMMAND ${ch1_test} --gtest_color=1 ${gtest_filter}   # Executable tests file name
)

# include(GoogleTest)
# gtest_discover_tests(${ch1_test})
