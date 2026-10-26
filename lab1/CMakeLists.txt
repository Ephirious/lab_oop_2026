cmake_minimum_required(VERSION 4.0)
project(lab1)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab1 main.cpp
        include/array.h
        src/array.cpp
        include/application.h
        src/application.cpp
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
        tests/test_array.cpp
        src/array.cpp
)

target_link_libraries(unit_tests PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)