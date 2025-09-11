#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include "types.hpp"

using namespace types;

namespace huffman_simple
{
    class Tree;

    constexpr u16 K_NULL_SYMBOL                     = UINT16_MAX;
    constexpr u8 K_CHECKED                          = 1;
    constexpr u8 K_UNCHECKED                        = 0;
    
    template <typename T>
    void LogToConsole(const T& msg)
    {
        std::cout << msg << std::endl;
    }

    class CodecIOState
    {
    public:
        explicit CodecIOState(const u8* const inputData, const usize inputByteWidth, const u8* const outputData, const usize maxOutputDataByteSize);
        explicit CodecIOState(const std::string& inputFileName, const std::string& outputFileName, const usize maxOutputDataByteSize);
        ~CodecIOState();

        inline Tree* GetTree() const { return _tree; }
        inline const std::string& GetOutputFilename() const { return _outputFilename; }
        inline u8* GetInputData() const { return _inputData; }
        inline u8* GetOutputData() const { return _outputData; }
        inline usize GetInputDataByteSize() const { return _inputDataByteSize; }
        inline usize GetOutputDataByteSize() const { return _outputDataByteSize; }
        inline usize GetMaxOutputDataByteSize() const { return _maxOutputDataByteSize; }

        inline void SetTree(const Tree* const tree) { _tree = (Tree*)tree; }
        inline void SetOutputDataByteSize(const usize size) { _outputDataByteSize = size; }

    private:
        Tree* _tree = nullptr;
        std::string _inputFilename = "";
        std::string _outputFilename = "";
        u8* _inputData = nullptr;
        u8* _outputData = nullptr;
        usize _inputDataByteSize = 0;
        usize _outputDataByteSize = 0;
        usize _maxOutputDataByteSize = 128 * 1024 * 1024; // 128 MiB
    };

    struct Symbol
    {
        u16 Char = K_NULL_SYMBOL;
        usize Freq = 0;
    };

    struct TreeNode
    {
        Symbol Symbol;
        u8 Flag = 0;
        u32 LeftIndex = 0;
        u32 RightIndex = 0;
        qword BitCode = 0;
        usize BitCodeBitSize = 0;
    };
    
    class Analyzer
    {
    public:
        explicit Analyzer(const CodecIOState& state);
        ~Analyzer() = default;

        inline usize GetUsedSymbolCount() const { return _usedSymbolCount; }
        inline Symbol* GetTable() { return &_table[0]; }

    private:
        usize _usedSymbolCount = 0;
        Symbol _table[256];
    };

    class Tree
    {
    public:
        explicit Tree(CodecIOState& state, Analyzer& analyzer);
        ~Tree();

        void AssignCodes(const u32 index, const usize bitCodeBitWidth, const qword bitCode);

        inline usize GetNodeCount() const { return _nodeCount; }
        inline TreeNode* GetRoot() const { return _tree; }
        inline const u32* GetSymbolTreeIndex() const { return &_symbolTreeIndex[0]; }

    private:
        usize _nodeCount = 0;
        TreeNode* _tree = nullptr;
        u32 _symbolTreeIndex[256] = {};
    };

    class BitWriter
    {
    public:
        explicit BitWriter(const u8* const writeAddress);
        ~BitWriter() = default;

        usize WriteBits(const usize count, const qword bits);
        void WriteBitsFromAddress(const usize byteCount, const u8* const address);
        void WriteRemainingBits();
        usize GetWrittenByteCount();

    private:
        const u8* _startAddress = nullptr;
        dword* _writeAddress = 0;
        qword _word = 0;
        usize _wordBitCount = 0;
    };

    class BitReader
    {
    public:
        explicit BitReader(const u8* const readAddress);
        ~BitReader() = default;

        qword ReadBits(const usize count, const qword mask, usize& bytesRead);
        void ReadBitsFromAddress(const usize byteCount, void* const address, usize& bytesRead);
        u16 ReadBitsWithTreeNodes(const u32 rootNodeIndex, const TreeNode* treeNodes, usize& bytesRead);
        usize GetReadByteCount();

    private:
        const u8* _startAddress = nullptr;
        dword* _readAddress = nullptr;
        usize _wordBitCount = 0;
        qword _word = 0;
    };

    class Encoder
    {
    public:
        explicit Encoder(CodecIOState& state);
        ~Encoder() = default;
    };

    class Decoder
    {
    public:
        explicit Decoder(CodecIOState& state);
        ~Decoder() = default;
    };
}