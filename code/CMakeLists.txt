cmake_minimum_required(VERSION 3.8.0)
project(fibers-case-study)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(Boost_ADDITIONAL_VERSIONS 1.66)
find_package(Boost 1.65.1 REQUIRED 
             COMPONENTS system context fiber)

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

# Add boost includes as global
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory_cond(currencies_with_fibers)
add_subdirectory_cond(context_explore)
add_subdirectory_cond(coro_explore)
add_subdirectory_cond(calculator)
add_subdirectory_cond(asio_explore)

