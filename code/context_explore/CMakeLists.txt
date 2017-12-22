project(context_explore)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/source)

add_executable(fibonacci ${SRC}/fibonacci.cpp)
target_link_libraries(fibonacci Threads::Threads ${Boost_LIBRARIES})

add_executable(passing_params ${SRC}/passing_params.cpp)
target_link_libraries(passing_params Threads::Threads ${Boost_LIBRARIES})

add_executable(resume_with ${SRC}/resume_with.cpp)
target_link_libraries(resume_with Threads::Threads ${Boost_LIBRARIES})

