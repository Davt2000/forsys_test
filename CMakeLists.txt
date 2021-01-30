cmake_minimum_required(VERSION 3.17)
project(forsys_test)

set(CMAKE_CXX_STANDARD 20)


add_executable(forsys_server server.cpp lib/zmq.hpp)

add_executable(forsys_client client.cpp lib/zmq.hpp)