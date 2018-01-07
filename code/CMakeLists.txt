cmake_minimum_required(VERSION 3.8.0)
project(fibers-case-study)

# Require c++17 std
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set globally used directories
set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(ARTIFACTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/artifacts)

# Find dependencies
set(Boost_ADDITIONAL_VERSIONS 1.66)
find_package(Boost 1.65.1 REQUIRED 
             COMPONENTS system context fiber)
find_package(Threads REQUIRED)

# Macro for conditional adding subproject
# subproject A will be added if we define USE_A or USE_ALL, 
# i.e. calling cmake -DUSE_ALL=1 all subprojects will be added
macro(add_subdirectory_cond SUBMODULE)
    if (USE_${SUBMODULE} OR USE_ALL)
        add_subdirectory(${SUBMODULE})
    endif()
endmacro()

# Add boost includes as global
include_directories(${Boost_INCLUDE_DIRS})
enable_testing()

# Subprojects
add_subdirectory_cond(fibers_explore)
add_subdirectory_cond(context_explore)
add_subdirectory_cond(coro_explore)
add_subdirectory_cond(asio_explore)
add_subdirectory_cond(benchmarks)

