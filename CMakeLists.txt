cmake_minimum_required(VERSION 3.2)

project(huffman_simple)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/huffman_simple.hpp
    src/types.hpp
    src/bit_reader.cpp
    src/bit_writer.cpp
    src/codec_io_state.cpp
    src/huff_encoder.cpp
    src/huff_decoder.cpp
    src/huff_analyzer.cpp
    src/huff_tree.cpp
    samples/example.cpp
)

add_executable(huffman_simple ${SOURCE_FILES})

if (MSVC)
    target_compile_options(huffman_simple PRIVATE /O3 /EHsc)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(huffman_simple PRIVATE -O3)
endif()