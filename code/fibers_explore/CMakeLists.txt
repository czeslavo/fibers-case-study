project(fibers_explore)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(poller ${SRC}/poller.cpp)
target_include_directories(poller PRIVATE ${EXTERNALS_DIR}/include)
target_link_libraries(poller Threads::Threads ${Boost_LIBRARIES} crypto ssl)

add_executable(hello ${SRC}/hello.cpp)
target_link_libraries(hello Threads::Threads ${Boost_LIBRARIES})

add_executable(buffered_channel ${SRC}/buffered_channel.cpp)
target_link_libraries(buffered_channel Threads::Threads ${Boost_LIBRARIES})

#add_executable(asio_files ${SRC}/asio_files.cpp)
#target_link_libraries(asio_files Threads::Threads ${Boost_LIBRARIES})

