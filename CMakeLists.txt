cmake_minimum_required(VERSION 3.29)
project(the-path-to-coroutines CXX)

find_package(Boost 1.84 COMPONENTS url system REQUIRED)

add_executable(single-flow)
target_sources(single-flow PRIVATE single-flow.cpp)
set_target_properties(single-flow PROPERTIES CXX_STANDARD 23)
target_link_libraries(single-flow Boost::boost)

add_executable(multi-thread)
target_sources(multi-thread PRIVATE multi-thread.cpp)
set_target_properties(multi-thread PROPERTIES CXX_STANDARD 23)
target_link_libraries(multi-thread Boost::boost)

add_executable(reactor)
target_sources(reactor PRIVATE reactor.cpp)
set_target_properties(reactor PROPERTIES CXX_STANDARD 23)
target_link_libraries(reactor Boost::boost)

add_executable(coroutines)
target_sources(coroutines PRIVATE coroutines.cpp)
set_target_properties(coroutines PROPERTIES CXX_STANDARD 23)
target_link_libraries(coroutines Boost::boost)
