cmake_minimum_required(VERSION 3.16)

project(algorithms_sedgewick)



# ----------------------
# Debugging
# ----------------------
set(DEBUG_MODE TRUE)



# ----------------------
# Build type
# ----------------------
if (NOT CMAKE_BUILD_TYPE AND DEBUG_MODE)
	set(CMAKE_BUILD_TYPE Debug)
elseif(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()



# ----------------------
# CXX settings
# ----------------------
set(CPP_COMPILER_STANDARD "-std=c++23")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# ----------------------
# Compiler flags
# ----------------------
# set(SANITIZER "-fsanitize=undefined,address")
set(EXCLUDED "-Wno-unused-function -Wno-sign-compare -Wno-sign-conversion")
# set(ERROR_LIMIT "-ferror-limit=0")
set(EXTRA_COMPILER_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wconversion -Wshadow -Wold-style-cast -Wpointer-arith -Wcast-qual ${EXCLUDED} ${ERROR_LIMIT}")

# Generate 'compile_commands.json' file required for 'clangd' and familiar tools.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${DEBUG_MODE})
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_COMPILER_STANDARD} ${EXTRA_COMPILER_FLAGS} ${COMPILATION_DATABASE} ${SANITIZER}")
	message(WARNING "Project is now in debug mode!")
else()
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_COMPILER_STANDARD} ${EXTRA_COMPILER_FLAGS} ${COMPILATION_DATABASE} ${SANITIZER}")
endif()



# ----------------------
# Conan >=2.0
# ----------------------
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)




# ----------------------
# External Libraries
# ----------------------
# find_package(ASIO REQUIRED)
find_package(FMT REQUIRED)
find_package(GTest REQUIRED)


# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)



# ----------------------
# Tests
# ----------------------
# Must be defined in top-most CMakeLists.txt file.
enable_testing()



# ----------------------
# Subdirectories
# ----------------------
add_subdirectory(tests)
add_subdirectory(src/ch1)




# ----------------------
# Create executable target
# ----------------------
add_executable(${PROJECT_NAME}  src/main.cpp)



# ----------------------
# Link libraries target with executable target
# ----------------------
target_link_libraries(${PROJECT_NAME} fmt::fmt ch1_lib)
