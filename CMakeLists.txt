cmake_minimum_required(VERSION 3.8)
project(ChessGame)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c engine.h)
add_executable(ChessGame ${SOURCE_FILES})