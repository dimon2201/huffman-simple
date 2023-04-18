cmake_minimum_required(VERSION 3.2)

project(huffman_simple)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

set(SOURCE_FILES
    src/BitReader.cpp
    src/BitWriter.cpp
    src/CodecIOState.cpp
    src/Decoder.cpp
    src/Encoder.cpp
    src/Sorter.cpp
    src/Tree.cpp
    samples/example.cpp
)

add_executable(huffman_simple ${SOURCE_FILES})