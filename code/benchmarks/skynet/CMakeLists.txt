project(skynet)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

macro(add_benchmark NAME)
    add_executable(${NAME} ${SRC}/${NAME}.cpp)
    target_link_libraries(${NAME} Threads::Threads ${Boost_LIBRARIES})
    install(TARGETS ${NAME} 
            DESTINATION ${CMAKE_SOURCE_DIR}/bin/benchmarks)
endmacro()

add_benchmark(skynet_fiber_detach)
add_benchmark(skynet_fiber_join)
add_benchmark(skynet_fiber_shared_detach)
add_benchmark(skynet_fiber_shared_join)
add_benchmark(skynet_async)
add_benchmark(skynet_thread)

install(FILES run.py 
        DESTINATION ${CMAKE_SOURCE_DIR}/bin/benchmarks
        PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE)
