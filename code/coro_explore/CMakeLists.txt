project(context_explore)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(generator ${SRC}/generator.cpp)
target_link_libraries(generator Threads::Threads ${Boost_LIBRARIES})

