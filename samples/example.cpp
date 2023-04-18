#include <iostream>
#include "../src/huffman_simple.hpp"

int main()
{
    huffman_simple::CodecIOState state("C:\\Users\\User\\Documents\\GitHub_Projects\\huffman_simple\\files\\enwik8",
        "C:\\Users\\User\\Documents\\GitHub_Projects\\huffman_simple\\files\\enwik8.huff");
    
    huffman_simple::Sorter sorter(state);
    huffman_simple::Tree tree(state, sorter);

    huffman_simple::Encoder encPass1(state);
    huffman_simple::Decoder decPass1(state);

    std::cout << "Success!" << std::endl;

    return 0;
}