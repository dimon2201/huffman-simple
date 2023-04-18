#include <fstream>
#include "huffman_simple.hpp"

huffman_simple::CodecIOState::CodecIOState(const void* inputData, const size_t inputByteWidth, const void* const outputData)
{
    HuffmanTree = nullptr;
    InputFileName = nullptr;
    InputData = inputData;
    InputByteWidth = inputByteWidth;
    OutputFileName = nullptr;
    OutputData = (void*)outputData;
    OutputByteWidth = 0;
}

huffman_simple::CodecIOState::CodecIOState(const char* inputFileName, const char* outputFileName)
{
    std::ifstream ifs(inputFileName, std::ios::in | std::ios::binary);
    if (!ifs) { return; }
    
    ifs.seekg(0, std::ios::end);
    InputByteWidth = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    InputData = malloc(InputByteWidth);
    ifs.read((char*)InputData, InputByteWidth);

    ifs.close();

    HuffmanTree = nullptr;
    OutputFileName = (char*)outputFileName;
    OutputData = malloc(128 * 1024 * 1024);
    OutputByteWidth = 0;
}

huffman_simple::CodecIOState::~CodecIOState()
{
    if (InputFileName != nullptr)
    {
        free((void*)InputData);
        free(OutputData);
    }
}