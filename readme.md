# IR COM

Embedded Autonomous Systems - Project

Participants: 
* Andreas Salzmann
* Daniel Caruso
* Jürgen Markl

WS 2022/23, HAW Landshut


## Scope

In this repository, the firmware for the MSP430FR5959 will be developed, which should act as a battery montiring device and communicates it's data to a collecting instance using a infrared interface. 

As the underlying protocoll, the IrDA-Stack shall be implemented using the IrDA lite version, as the full stack would contain too much features, that are not needed for this simple communication. 

## Project structure

in library_dev the IrDA Stack will be developed

in msp430_workspace, a workspace for the Code Composer Studio is created and the integration will happen in there




## Unit Tests

to build unit tests: execute the following statements (linux)

    mkdir build && cd build

    cmake ..

    cmake --build .

    ./ctest notifynotify


to extend the unit tests: 

Add a test case in tests directory by following tutorials, e.g. [GoogleTest](https://google.github.io/googletest/quickstart-cmake.html)

Add the test case to the CMakeLists.txt in the tests/ directory

    add_exectuable(test_name test_file.cpp)
    target_link_libraries(test_name GTest::gtest_main)
    add_test(test_name2 test_name)


