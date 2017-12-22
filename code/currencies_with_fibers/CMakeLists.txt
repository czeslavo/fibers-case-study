project(currencies_with_fibers)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/source)
add_executable(poller ${SRC}/poller.cpp)
target_include_directories(poller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                            ${EXTERNALS_DIR}/include)
target_link_libraries(poller Threads::Threads ${Boost_LIBRARIES} crypto ssl)
