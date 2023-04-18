#include <fstream>
#include "huffman_simple.hpp"

huffman_simple::Decoder::Decoder(CodecIOState& state)
{
    uint8_t* inputData = (uint8_t*)state.OutputData;

    // Read metadata
    uint64_t treeByteWidth = ((uint32_t*)inputData)[0];
    uint64_t tableByteWidth = ((uint32_t*)inputData)[1];
    uint64_t inputSymbolCount = ((uint32_t*)inputData)[2];
    inputData += 12;
    
    uint8_t* outputData = (uint8_t*)state.InputData;
    const TreeNode* tree = (const TreeNode*)malloc(treeByteWidth);
    const uint32_t* table = (const uint32_t*)malloc(tableByteWidth);
    memcpy((void*)tree, inputData, treeByteWidth);
    inputData += treeByteWidth;
    memcpy((void*)table, inputData, tableByteWidth);
    inputData += tableByteWidth;

    // Decode Huffman codes to original fixed codes
    BitReader br(inputData);
    const size_t nodeCount = treeByteWidth / sizeof(TreeNode);
    const uint32_t rootNodeIndex = nodeCount - 1;
    for (size_t i = 0; i < inputSymbolCount; i++)
    {
        uint16_t symbol = br.ReadBitsWithTreeNodes(rootNodeIndex, tree);
        outputData[i] = (uint8_t)symbol;
    }

    free((void*)tree);
    free((void*)table);

    // Output to file if needed
    if (state.OutputFileName == nullptr) { return; }

    std::string outputFileName = std::string(state.OutputFileName) + ".output";
    std::ofstream ofs(outputFileName.c_str(), std::ios::out | std::ios::binary);
    if (!ofs) { return; }

    ofs.write((const char*)outputData, inputSymbolCount);
    ofs.close();
}

huffman_simple::Decoder::~Decoder()
{

}