cmake_minimum_required(VERSION 3.16)
project(simulation C)

set(CMAKE_C_STANDARD 99)

add_executable(simulation main.c CCSDS.c)