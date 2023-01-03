cmake_minimum_required(VERSION 3.14)
project(ircom_msp)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)




include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)




enable_testing()

include_directories(library_dev)
# include_directories(/opt/ccs/ccs1210/ccs/ccs_base/msp430/include/)
# include_directories(/opt/ccs/ccs1210/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include/)

add_subdirectory(library_dev)
add_subdirectory(tests)

