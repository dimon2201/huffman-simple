#include <fstream>
#include "huffman_simple.hpp"

huffman_simple::Encoder::Encoder(CodecIOState& state)
{
    uint8_t* outputData = (uint8_t*)state.OutputData;

    // Write metadata
    const Tree* tree = state.HuffmanTree;
    const uint32_t treeByteWidth = sizeof(TreeNode) * tree->_nodeCount;
    const uint32_t tableByteWidth = sizeof(uint32_t) * 256;
    const size_t inputSymbolCount = state.InputByteWidth;
    ((uint32_t*)outputData)[0] = treeByteWidth;
    ((uint32_t*)outputData)[1] = tableByteWidth;
    ((uint32_t*)outputData)[2] = inputSymbolCount;
    outputData += 12;
    memcpy(outputData, tree->_tree, treeByteWidth);
    outputData += treeByteWidth;
    memcpy(outputData, tree->_symbolTreeIndex, tableByteWidth);
    outputData += tableByteWidth;

    // Encode input data into Huffman codes
    BitWriter bw(outputData);
    const uint8_t* inputData = (const uint8_t*)state.InputData;
    for (size_t i = 0; i < state.InputByteWidth; ++i)
    {
        const uint32_t treeIndex = tree->_symbolTreeIndex[inputData[i]];
        const size_t bitCodeBitWidth = tree->_tree[treeIndex].BitCodeBitWidth;
        const uint64_t bitCode = tree->_tree[treeIndex].BitCode;

        bw.WriteBits(bitCodeBitWidth, bitCode);
    }

    bw.WriteRemainingBits();
    state.OutputByteWidth = 12 + treeByteWidth + tableByteWidth + bw.GetWrittenByteCount();

    // Output to file if needed
    if (state.OutputFileName == nullptr) { return; }

    std::ofstream ofs(state.OutputFileName, std::ios::out | std::ios::binary);
    if (!ofs) { return; }

    const uint32_t outputByteWidth = (const uint32_t)state.OutputByteWidth;
    ofs.write((const char*)state.OutputData, outputByteWidth);
    ofs.close();
}

huffman_simple::Encoder::~Encoder()
{

}