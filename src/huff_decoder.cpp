#include <fstream>
#include "huffman_simple.hpp"

huffman_simple::Decoder::Decoder(CodecIOState& state)
{
    const std::string& outputFilename = state.GetOutputFilename();
    const std::string decodedOutputFileName = outputFilename + ".output";
    if (outputFilename == "")
        LogToConsole("Decoding output data...");
    else
        LogToConsole("Decoding output data to '" + decodedOutputFileName + "'...");

    u8* inputData = (u8*)state.GetOutputData();

    // Read metadata
    const usize treeByteSize = ((uint32_t*)inputData)[0];
    const usize tableByteSize = ((uint32_t*)inputData)[1];
    const usize inputSymbolCount = ((uint32_t*)inputData)[2];
    inputData += 12;
    
    u8* const outputData = (u8* const)state.GetInputData();
    const TreeNode* tree = (const TreeNode*)malloc(treeByteSize);
    const uint32_t* table = (const uint32_t*)malloc(tableByteSize);
    memcpy((void*)tree, inputData, treeByteSize);
    inputData += treeByteSize;
    memcpy((void*)table, inputData, tableByteSize);
    inputData += tableByteSize;

    // Decode Huffman codes to original fixed codes
    BitReader br(inputData);
    const usize nodeCount = treeByteSize / sizeof(TreeNode);
    const u32 rootNodeIndex = (u32)nodeCount - 1;
    for (usize i = 0; i < inputSymbolCount; i++)
    {
        usize bytesRead = 0;
        u16 symbol = br.ReadBitsWithTreeNodes(rootNodeIndex, tree, bytesRead);
        outputData[i] = (u8)symbol;
        if (bytesRead >= K_MAX_OUTPUT_BUFFER_BYTE_SIZE)
        {
            LogToConsole("Error: Decoded input data byte size '" + std::to_string(bytesRead) + "' exceeds limit '" + std::to_string(K_MAX_OUTPUT_BUFFER_BYTE_SIZE) + "'! The program's behavior is undefined!");
            break;
        }
    }
    free((void*)tree);
    free((void*)table);

    // Output to file if needed
    if (outputFilename == "")
        return;

    std::ofstream ofs(decodedOutputFileName.c_str(), std::ios::out | std::ios::binary);
    if (!ofs)
    {
        LogToConsole("Error: Output file is invalid! The program's behavior is undefined!");
        return;
    }
    ofs.write((const char*)outputData, inputSymbolCount);
    ofs.close();

    LogToConsole("    Success");
}