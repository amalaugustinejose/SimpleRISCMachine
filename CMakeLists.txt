cmake_minimum_required(VERSION 3.14)
project(riscMachine)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GoogleTest)
find_package(GTest REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/tests_gtest")

# create the main app
set(SOURCE_FILES
  src/instr_set.h
  src/LRUCache.h src/LRUCache.cpp
  src/memory.h src/memory.cpp
  src/cpu.cpp src/cpu.h)
add_executable(riscMachine src/main.cpp ${SOURCE_FILES})

add_executable(
  testSystem ${SOURCE_FILES}
  test_gtest/test_system.cpp
)
target_link_libraries(
  testSystem
  GTest::gtest_main
  GTest::gtest
)

gtest_add_tests(TARGET testSystem)
