cmake_minimum_required(VERSION 3.15)
project(forsys_test)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

add_executable(forsys_server server.cpp common.hpp)
target_include_directories(forsys_server PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(forsys_server PUBLIC ${ZeroMQ_LIBRARY})



add_executable(forsys_client client.cpp common.hpp)
target_include_directories(forsys_client PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(forsys_client PUBLIC ${ZeroMQ_LIBRARY})