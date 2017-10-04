cmake_minimum_required(VERSION 3.8.0)
project(fibers-case-study)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
find_package(Boost 1.62 REQUIRED 
             COMPONENTS fiber system)
find_package(Threads REQUIRED)

macro(add_subdirectory_cond SUBMODULE)
    if(USE_${SUBMODULE} OR USE_ALL)
        add_subdirectory(${SUBMODULE})
    endif()
endmacro()

add_subdirectory_cond(fibers-1)
