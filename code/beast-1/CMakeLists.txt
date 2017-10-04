project(fibers-1)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(fibers-1 ${SOURCE_FILES})
target_include_directories(fibers-1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                            ${EXTERNALS_DIR}/beast/include)
target_link_libraries(fibers-1 Threads::Threads ${Boost_LIBRARIES})


