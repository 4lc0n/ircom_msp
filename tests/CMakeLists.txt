

add_executable(multiply_test multiply_test.cpp)

target_link_libraries(multiply_test
                GTest::gtest_main)


add_test(multiply_gtests multiply_test)


add_executable(irlap_test irlap_test.cpp)
add_compile_definitions(irlap_test PRIVATE UNIT_TESTING=1)
target_link_libraries(irlap_test GTest::gtest_main)

add_test(irlap_test irlap_test)


