enable_testing()

add_executable(blackjack_tests blackjack_unittest.cpp)

target_compile_options(blackjack_tests PRIVATE
    -Wno-template-body
)

target_link_libraries(blackjack_tests PRIVATE
    blackjack_core
    CppUTest::CppUTest
    CppUTest::CppUTestExt
)

add_test(NAME blackjack_tests COMMAND blackjack_tests)
