project(aggregation)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(aggregation_fibers ${SRC}/fibers.cpp)
target_link_libraries(aggregation_fibers Threads::Threads ${Boost_LIBRARIES})
