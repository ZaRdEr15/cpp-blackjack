cmake_minimum_required(VERSION 3.15)

project(cpp-blackjack CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF) # will compile correctly on any c++ compiler

add_compile_options(
    -Wall
    -Wextra
)

add_library(blackjack_core src/blackjack.cpp)
target_include_directories(blackjack_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(blackjack src/main.cpp)
target_link_libraries(blackjack PRIVATE blackjack_core)

target_link_options(blackjack PUBLIC
    $<$<CONFIG:Release>:-static>
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")

    include(FetchContent)
    FetchContent_Declare(
        CppUTest
        GIT_REPOSITORY https://github.com/cpputest/cpputest.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(CppUTest)

    enable_testing()

    add_executable(blackjack_tests tests/blackjack_unittest.cpp)

    target_compile_options(blackjack_tests PRIVATE
        -Wno-template-body
    )

    target_link_libraries(blackjack_tests PRIVATE
        blackjack_core
        CppUTest::CppUTest
        CppUTest::CppUTestExt
    )

    add_test(NAME blackjack_tests COMMAND blackjack_tests)

endif()
