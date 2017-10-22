project(calculator)

# Calculator
file(GLOB_RECURSE CALCULATOR_SRC ${CMAKE_CURRENT_SOURCE_DIR}/source/calculator/*.cpp)
add_executable(calculator ${CALCULATOR_SRC})
target_include_directories(calculator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/calculator)
target_link_libraries(calculator Threads::Threads ${Boost_LIBRARIES})

# Tests
if (WITH_TESTS)
    file(GLOB_RECURSE TESTS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    add_executable(calculator_tests ${TESTS_SRC})
    target_include_directories(calculator_tests PRIVATE ${gtest_SOURCE_DIR}/include
                                                PRIVATE ${gmock_SOURCE_DIR}/include
                                                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(calculator_tests ${Boost_LIBRARIES} gtest_main)
    add_test(CalculatorTests calculator_tests)
endif()


