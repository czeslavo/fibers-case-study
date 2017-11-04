project(asio-explore)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

add_executable(asio-timers ${SRC_DIR}/timers.cpp)
target_link_libraries(asio-timers Threads::Threads ${Boost_LIBRARIES})

add_executable(daytime-client ${SRC_DIR}/daytime_client.cpp)
target_link_libraries(daytime-client Threads::Threads ${Boost_LIBRARIES})

add_executable(daytime-server ${SRC_DIR}/daytime_server.cpp)
target_link_libraries(daytime-server Threads::Threads ${Boost_LIBRARIES})

add_executable(roman-numerals-client ${SRC_DIR}/roman_numerals_client.cpp)
target_link_libraries(roman-numerals-client Threads::Threads ${Boost_LIBRARIES})

add_executable(roman-numerals-server ${SRC_DIR}/roman_numerals_server.cpp)
target_link_libraries(roman-numerals-server Threads::Threads ${Boost_LIBRARIES})
