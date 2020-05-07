cmake_minimum_required(VERSION 3.5)

project(huffman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(huffman main.cpp tree.cpp)
