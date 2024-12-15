cmake_minimum_required(VERSION 3.27)
project(HW3)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic-errors -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -Wall")

add_executable(HW1
        main25a1.cpp
        plains25a1.cpp
        plains25a1.h
        wet1util.h)
