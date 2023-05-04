cmake_minimum_required(VERSION 3.10)
# Updated to 3.22 to match the cmake version on CSIF
# Defines the minimum required cmake version
# You can check cmake version on your os by typing 
# `cmake --version` on the terminal

project(Enigma VERSION 1.1.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_C_STANDARD 99)


set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
    CACHE STRING "Choose the type of build, options are: None Debug Release asan"
    FORCE)
# AddressSanitize
set(CMAKE_C_FLAGS_ASAN
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1 -Wall -Wextra -Wpedantic -Werror"
    CACHE STRING "Flags used by the C compiler during AddressSanitizer builds."
    FORCE)
set(CMAKE_CXX_FLAGS_ASAN
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1 -Wall -Wextra -Wpedantic -Werror"
    CACHE STRING "Flags used by the C++ compiler during AddressSanitizer builds."
    FORCE)

include_directories(${INCLUDE_DIR})
add_library(Enigma SHARED
    ${LIB_DIR}/Rotor.c
    ${LIB_DIR}/Plugboard.c
    ${LIB_DIR}/Reflector.c
    ${LIB_DIR}/Enigma.c
)


# The code below sets up GoogleTest
# Reference: http://google.github.io/googletest/quickstart-cmake.html

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()

add_executable(EnigmaTests
  tests/RotorTests.cpp
  tests/PlugboardTests.cpp
  tests/ReflectorTests.cpp
  tests/EnigmaTests.cpp
  tests/ExampleTests.cpp
  tests/AutograderTests.cpp
)
target_link_libraries(EnigmaTests
  Enigma
  GTest::gtest_main
)

gtest_discover_tests(EnigmaTests)