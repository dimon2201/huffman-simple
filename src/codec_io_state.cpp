#include <fstream>
#include "huffman_simple.hpp"

huffman_simple::CodecIOState::CodecIOState(const u8* const inputData, const usize inputDataByteSize, const u8* const outputData)
{
    LogToConsole("Setting up codec state...");

    if (inputDataByteSize >= K_MAX_OUTPUT_BUFFER_BYTE_SIZE)
        LogToConsole("Error: Input data byte size '" + std::to_string(inputDataByteSize) + "' exceeds limit '" + std::to_string(K_MAX_OUTPUT_BUFFER_BYTE_SIZE) + "'! The program's behavior is undefined!");

    _inputData = (u8*)inputData;
    _outputData = (u8*)outputData;
    _inputDataByteSize = inputDataByteSize;
    _outputDataByteSize = 0;

    LogToConsole("    Success");
}

huffman_simple::CodecIOState::CodecIOState(const std::string& inputFileName, const std::string& outputFileName)
{
    std::ifstream ifs(inputFileName, std::ios::in | std::ios::binary);
    if (!ifs)
    {
        LogToConsole("Error: Input file is invalid! The program's behavior is undefined!");
        return;
    }
    
    ifs.seekg(0, std::ios::end);
    _inputDataByteSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    _inputData = (u8*)malloc(_inputDataByteSize);
    ifs.read((char*)_inputData, _inputDataByteSize);
    ifs.close();

    _inputFilename = inputFileName;
    _outputFilename = outputFileName;
    _outputData = (u8*)malloc(K_MAX_OUTPUT_BUFFER_BYTE_SIZE);
    _outputDataByteSize = 0;

    CodecIOState(_inputData, _inputDataByteSize, _outputData);
}

huffman_simple::CodecIOState::~CodecIOState()
{
    if (_inputFilename != "")
    {
        free(_inputData);
        free(_outputData);
    }
}