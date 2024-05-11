# Set TARGET(binary file) name for convenience
set(<TEST_VAR> ${PROJECT_NAME}_<TEST_NAME>)

# Test files
set(
        TEST_FILES
        <FILE_TO_TEST>.cpp
#        test_temperature_reading.cpp
)

# (Names of) Tested libraries
set(
        TESTED_LIBRARIES
#        ${project_name}_ch10_homework
#        ${project_name}_exception
)

# Executable that runs the tests.
add_executable(
        ${<TEST_VAR>}             # Executable tests file name
        test_main.cpp
        ${TEST_FILES}
)

# Link the executable to needed libraries.
target_link_libraries(
        ${<TEST_VAR>}
        ${TESTED_LIBRARIES}     # Library we are testing
        gtest gtest_main        # GTest libraries
)

# Add gtest to be able to run ctest.
add_test(
        NAME CH10Test            # Test name
        COMMAND ${<TEST_VAR>} --gtest_color=1     # Executable tests file name
)