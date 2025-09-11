#include <chrono>
#include "../src/huffman_simple.hpp"

int main()
{
    std::string inputFilename;
    std::string outputFilename;

    std::cout <<
        "Huffman Codec Example App" << std::endl <<
        "Repository: https://github.com/dimon2201/huffman-simple" << std::endl <<
        "Author: dimon2201" << std::endl << std::endl;

    std::cout << "Input filename: ";
    std::getline(std::cin, inputFilename);
    std::cout << "Output filename: ";
    std::getline(std::cin, outputFilename);
    std::cout << std::endl;

    if (inputFilename == "" || outputFilename == "")
        return 1;

	auto start = std::chrono::high_resolution_clock::now();
	
    huffman_simple::CodecIOState state(inputFilename, outputFilename);
    huffman_simple::Analyzer analyzer(state);
    huffman_simple::Tree tree(state, analyzer);
    huffman_simple::Encoder encPass1(state);
    huffman_simple::Decoder decPass1(state);
	
	auto end = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Program succeeded!" << std::endl;
	std::cout << "Elapsed time: " << time << " ms" << std::endl;

    return 0;
}