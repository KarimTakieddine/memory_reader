cmake_minimum_required(VERSION 4.3.3 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20 REQUIRED)

set(PROJECT_NAME memory_reader)
set(TEST_DIR test)

list(APPEND SOURCES
    ${TEST_DIR}/test_memory_view.cpp)

project(${PROJECT_NAME})

enable_testing()

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
