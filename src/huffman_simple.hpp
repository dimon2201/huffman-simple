#pragma once

#include <cstdint>
#include <vector>

namespace huffman_simple
{
    class Tree;
    class Encoder;

    constexpr uint16_t NULL_SYMBOL = UINT16_MAX;
    constexpr uint8_t CHECKED = 1;
    constexpr uint8_t UNCHECKED = 0;

    struct CodecIOState
    {
        CodecIOState(const void* inputData, const size_t inputByteWidth, const void* const outputData);
        CodecIOState(const char* inputFileName, const char* outputFileName);
        ~CodecIOState();

        Tree* HuffmanTree;
        char* InputFileName;
        const void* InputData;
        size_t InputByteWidth;
        char* OutputFileName;
        void* OutputData;
        size_t OutputByteWidth;
    };

    struct Symbol
    {
        uint16_t Char;
        size_t Freq;
    };

    struct TreeNode
    {
        Symbol Symbol;
        uint8_t Flag;
        uint32_t LeftIndex;
        uint32_t RightIndex;
        size_t BitCodeBitWidth;
        uint64_t BitCode;
    };
    
    class Sorter
    {
        friend Tree;

        public:
            explicit Sorter(CodecIOState& state);
            ~Sorter();

        private:
            size_t _presentSymbolCount;
            Symbol _table[256];
    };

    class Tree
    {
        friend Encoder;

        public:
            explicit Tree(CodecIOState& state, Sorter& sorter);
            ~Tree();

            void AssignCodes(uint32_t index, size_t bitCodeBitWidth, uint64_t bitCode);

        private:
            size_t _nodeCount;
            TreeNode* _tree;
            uint32_t _symbolTreeIndex[256];
    };

    class BitWriter
    {
        public:
            explicit BitWriter(const void* const writeAddress);
            ~BitWriter();

            void WriteBits(const size_t count, const uint64_t bits);
            void WriteBitsFromAddress(const size_t byteCount, const void* address);
            void WriteRemainingBits();
            size_t GetWrittenByteCount();

        private:
            const void* _startAddress;
            uint32_t* _writeAddress;
            size_t _wordBitCount;
            uint64_t _word;
    };

    class BitReader
    {
        public:
            explicit BitReader(const void* const readAddress);
            ~BitReader();

            uint64_t ReadBits(size_t count, uint64_t mask);
            void ReadBitsFromAddress(const size_t byteCount, void* const address);
            uint16_t ReadBitsWithTreeNodes(const uint32_t rootNodeIndex, const TreeNode* treeNodes);
            size_t GetReadByteCount();

        private:
            const void* _startAddress;
            uint32_t* _readAddress;
            size_t _wordBitCount;
            uint64_t _word;
    };

    class Encoder
    {
        public:
            explicit Encoder(CodecIOState& state);
            ~Encoder();
    };

    class Decoder
    {
        public:
            explicit Decoder(CodecIOState& state);
            ~Decoder();
    };
}