project(skynet)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

# Adds executable NAME with source NAME.cpp, links it to boost
# and installs it in benchmarks artifacts dir
macro(add_benchmark NAME)
    add_executable(${NAME} ${SRC}/${NAME}.cpp)
    target_link_libraries(${NAME} Threads::Threads ${Boost_LIBRARIES})
    install(TARGETS ${NAME} 
            DESTINATION ${ARTIFACTS_DIR}/benchmarks)
endmacro()

add_benchmark(skynet_fiber_detach)
add_benchmark(skynet_fiber_join)
add_benchmark(skynet_fiber_shared_detach)
add_benchmark(skynet_fiber_shared_join)
add_benchmark(skynet_async)
add_benchmark(skynet_thread)

install(FILES run_skynet.py 
        DESTINATION ${ARTIFACTS_DIR}/benchmarks
        PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE)

add_test(NAME skynet_10k COMMAND ${SRC}/run_skynet.py 10000)
add_test(NAME skynet_1M COMMAND ${SRC}/run_skynet.py 1000000)
