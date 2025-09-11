#include "huffman_simple.hpp"

huffman_simple::BitReader::BitReader(const u8* const readAddress)
{
    _startAddress = readAddress;
    _readAddress = (dword*)_startAddress;
    _wordBitCount = 0;
    _word = 0;
}

qword huffman_simple::BitReader::ReadBits(const usize count, const qword mask, usize& bytesRead)
{
    while (_wordBitCount < 32)
    {
        _word |= (usize)_readAddress[0] << _wordBitCount;
        _readAddress++;
        _wordBitCount += 32;
    }

    qword bits = _word & mask;
    _word >>= count;
    _wordBitCount -= count;

    bytesRead = (u8*)_readAddress - _startAddress;

    return bits;
}

void huffman_simple::BitReader::ReadBitsFromAddress(const usize byteCount, void* const address, usize& bytesRead)
{
    u8* const typedAddress = (u8* const)address;
    for (usize i = 0; i < byteCount; ++i)
    {
        qword bits = ReadBits(8, 0xff, bytesRead);
        typedAddress[i] = bits & 0xff;
    }
}

uint16_t huffman_simple::BitReader::ReadBitsWithTreeNodes(const u32 rootNodeIndex, const TreeNode* treeNodes, usize& bytesRead)
{
    qword bitCode = 0;
    usize bitCodeBitSize = 0;
    u32 curIndex = rootNodeIndex;
    u16 symbol;
    do
    {
        const u8 bit = (u8)ReadBits(1, 1, bytesRead);

        if (bit == 0)
            curIndex = treeNodes[curIndex].LeftIndex;
        else
            curIndex = treeNodes[curIndex].RightIndex;

        symbol = treeNodes[curIndex].Symbol.Char;

        bitCode |= bit << bitCodeBitSize;
        bitCodeBitSize++;
    } while (symbol == K_NULL_SYMBOL);

    return symbol;
}

usize huffman_simple::BitReader::GetReadByteCount()
{
    return (usize)_readAddress - (usize)_startAddress;
}