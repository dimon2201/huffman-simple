#include "huffman_simple.hpp"

huffman_simple::BitReader::BitReader(const void* const readAddress)
{
    _startAddress = readAddress;
    _readAddress = (uint32_t*)_startAddress;
    _wordBitCount = 0;
    _word = 0;
}

huffman_simple::BitReader::~BitReader()
{
    
}

uint64_t huffman_simple::BitReader::ReadBits(size_t count, uint64_t mask)
{
    while (_wordBitCount < 32)
    {
        _word |= (uint64_t)_readAddress[0] << (uint64_t)_wordBitCount;
        _readAddress += 1;
        _wordBitCount += 32;
    }

    uint64_t bits = _word & mask;
    _word >>= (uint64_t)count;
    _wordBitCount -= count;

    return bits;
}

void huffman_simple::BitReader::ReadBitsFromAddress(const size_t byteCount, void* const address)
{
    uint8_t* const typedAddress = (uint8_t* const)address;
    for (size_t i = 0; i < byteCount; ++i)
    {
        uint64_t bits = ReadBits(8, 0xff);

        typedAddress[i] = bits & 0xff;
    }
}

uint16_t huffman_simple::BitReader::ReadBitsWithTreeNodes(const uint32_t rootNodeIndex, const TreeNode* treeNodes)
{
    uint64_t bitCode = 0;
    size_t bitCodeBitWidth = 0;
    uint32_t curIndex = rootNodeIndex;
    uint16_t symbol;
    do
    {
        uint8_t bit = ReadBits(1, 1);

        if (bit == 0) {
            curIndex = treeNodes[curIndex].LeftIndex;
        } else {
            curIndex = treeNodes[curIndex].RightIndex;
        }

        symbol = treeNodes[curIndex].Symbol.Char;

        bitCode |= bit << bitCodeBitWidth;
        bitCodeBitWidth += 1;
    } while (symbol == NULL_SYMBOL);

    return symbol;
}

size_t huffman_simple::BitReader::GetReadByteCount()
{
    return (size_t)_readAddress - (size_t)_startAddress;
}