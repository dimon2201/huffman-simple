#include <fstream>
#include "huffman_simple.hpp"

huffman_simple::Encoder::Encoder(CodecIOState& state)
{
    LogToConsole("Encoding input data...");

    u8* outputData = (u8*)state.GetOutputData();

    // Write metadata
    const Tree* const tree = state.GetTree();
    const TreeNode* root = tree->GetRoot();
    const u32* symbolTreeIndex = tree->GetSymbolTreeIndex();
    const u32 treeByteWidth = (u32)sizeof(TreeNode) * (u32)tree->GetNodeCount();
    const u32 tableByteWidth = sizeof(u32) * 256;
    const usize inputSymbolCount = state.GetInputDataByteSize();
    ((u32*)outputData)[0] = treeByteWidth;
    ((u32*)outputData)[1] = tableByteWidth;
    ((u32*)outputData)[2] = (u32)inputSymbolCount;
    outputData += 12;
    memcpy(outputData, root, treeByteWidth);
    outputData += treeByteWidth;
    memcpy(outputData, symbolTreeIndex, tableByteWidth);
    outputData += tableByteWidth;

    // Encode input data into Huffman codes
    BitWriter bw(outputData);
    const u8* const inputData = (const u8* const)state.GetInputData();
    for (usize i = 0; i < inputSymbolCount; ++i)
    {
        const u32 treeIndex = symbolTreeIndex[inputData[i]];
        const qword bitCode = root[treeIndex].BitCode;
        const usize bitCodeBitWidth = root[treeIndex].BitCodeBitSize;
        const usize bytesWritten = bw.WriteBits(bitCodeBitWidth, bitCode);
        if (bytesWritten >= K_MAX_OUTPUT_BUFFER_BYTE_SIZE)
        {
            LogToConsole("Encoded output data byte size '" + std::to_string(bytesWritten) + "' exceeds limit '" + std::to_string(K_MAX_OUTPUT_BUFFER_BYTE_SIZE) + "'! The program's behavior is undefined!");
            break;
        }
    }

    bw.WriteRemainingBits();
    state.SetOutputDataByteSize(12 + treeByteWidth + tableByteWidth + bw.GetWrittenByteCount());

    // Output to file if needed
    const std::string& outputFilename = state.GetOutputFilename();
    if (outputFilename == "")
        return;

    std::ofstream ofs(outputFilename, std::ios::out | std::ios::binary);
    if (!ofs)
        return;

    const usize outputDataByteSize = state.GetOutputDataByteSize();
    ofs.write((const char*)outputData, outputDataByteSize);
    ofs.close();

    LogToConsole("    Success");
}