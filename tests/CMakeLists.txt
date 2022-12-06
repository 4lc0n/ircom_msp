

add_executable(multiply_test multiply_test.cpp)

target_link_libraries(multiply_test
                GTest::gtest_main)

add_test(multiply_gtests multiply_test)