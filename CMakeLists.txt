cmake_minimum_required(VERSION 4.1.2)

project(algorithms_sedgewick)

# Set isysroot (MacOS)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    # The path is result of "xcrun --show-sdk-path" command
    set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")
endif()


# ----------------------
# CXX settings
# ----------------------
set(CPP_COMPILER_STANDARD "-std=c++23")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------
# Compiler flags
# ----------------------
set(EXCLUDED "-Wno-sign-conversion")
# set(ERROR_LIMIT "-ferror-limit=0")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(SANITIZER "-fsanitize=undefined,leak,address -fsanitize-undefined-trap-on-error")
# elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
#   set(SANITIZER "-fsanitize=address,undefined")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set(CLANG_FLAGS "-Wcovered-switch-default -Wgnu -Wmicrosoft -Wmost ${ERROR_LIMIT}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(GCC_FLAGS "-fmodules-ts")
endif()
set(EXTRA_COMPILER_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wconversion -Wunreachable-code -Wcast-align -Wshadow -Wmissing-noreturn -Wold-style-cast -Wpointer-arith -Wcast-qual ${CLANG_FLAGS} ${GCC_FLAGS} ${EXCLUDED}")

# ----------------------
# Debugging
# ----------------------
set(DEBUG_MODE FALSE)

# ----------------------
# Build type
# ----------------------
if (NOT CMAKE_BUILD_TYPE AND DEBUG_MODE)
	set(CMAKE_BUILD_TYPE Debug)
elseif(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

if(${DEBUG_MODE})
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(USER_DEBUG_FLAGS "-gdwarf-4") # Linux only
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_COMPILER_STANDARD} ${EXTRA_COMPILER_FLAGS} ${COMPILATION_DATABASE} ${SANITIZER}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g ${USER_DEBUG_FLAGS}")
  message(WARNING "Debug mode is on!")
else()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_COMPILER_STANDARD} ${EXTRA_COMPILER_FLAGS} ${COMPILATION_DATABASE} ${SANITIZER}")
endif()


# Generate 'compile_commands.json' file required for 'clangd' and familiar tools.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
# add_executable(${PROJECT_NAME}  src/main.cpp)


# ----------------------
# Link libraries target with executable target
# ----------------------
# target_link_libraries(${PROJECT_NAME} fmt::fmt ch1_lib)
