cmake_minimum_required(VERSION 4.0)
project(lab2)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab2 main.cpp
        src/polynomial.cpp
        include/polynomial.h
        tests/polynomial_test.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
        tests/polynomial_test.cpp
        src/polynomial.cpp
)

target_link_libraries(unit_tests PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
