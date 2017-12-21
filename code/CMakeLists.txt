cmake_minimum_required(VERSION 3.8.0)
project(fibers-case-study)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
find_package(Boost 1.65.1 REQUIRED 
             COMPONENTS system context)
find_package(Threads REQUIRED)

macro(add_subdirectory_cond SUBMODULE)
    if (USE_${SUBMODULE} OR USE_ALL)
        add_subdirectory(${SUBMODULE})
    endif()
endmacro()

if (WITH_TESTS)
    add_subdirectory(${EXTERNALS_DIR}/googletest)
    enable_testing()
endif()

add_subdirectory_cond(beast-1)
add_subdirectory_cond(context-1)
add_subdirectory_cond(calculator)
add_subdirectory_cond(asio-explore)

