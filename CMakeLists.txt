cmake_minimum_required(VERSION 3.10)
project(vector C)

set(CMAKE_C_STANDARD 99)

add_executable(server main.c src/vector.c inc/vector.h src/server.c inc/server.h)

target_link_libraries(server pthread)
