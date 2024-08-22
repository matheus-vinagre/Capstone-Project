cmake_minimum_required(VERSION 2.6)
project(monitor)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

find_package(Boost 1.58 REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES}
        include/memory.h
        src/memory.cpp)

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES} ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(monitor ${CMAKE_THREAD_LIBS_INIT})

target_compile_options(monitor PRIVATE -Wall -Wextra)