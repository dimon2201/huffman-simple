#include "huffman_simple.hpp"

huffman_simple::BitWriter::BitWriter(const void* const writeAddress)
{
    _startAddress = writeAddress;
    _writeAddress = (uint32_t*)_startAddress;
    _wordBitCount = 0;
    _word = 0;
}

huffman_simple::BitWriter::~BitWriter()
{

}

void huffman_simple::BitWriter::WriteBits(const size_t count, const uint64_t bits)
{
    while (_wordBitCount >= 32)
    {
        _writeAddress[0] = _word & 0xffffffff;
        _writeAddress += 1;
        _wordBitCount -= 32;
        _word >>= 32;
    }

    _word |= (uint64_t)(bits) << (uint64_t)_wordBitCount;
    _wordBitCount += count;
}

void huffman_simple::BitWriter::WriteBitsFromAddress(const size_t byteCount, const void* address)
{
    const uint8_t* typedAddress = (const uint8_t*)address;
    for (size_t i = 0; i < byteCount; ++i)
    {
        WriteBits(8, (const uint64_t)typedAddress[i]);
    }
}

void huffman_simple::BitWriter::WriteRemainingBits()
{
    if (_wordBitCount > 0)
    {
        _writeAddress[0] = _word & 0xffffffff;
        _writeAddress += 1;
        _word >>= 32;
        _wordBitCount = 0;
    }
}

size_t huffman_simple::BitWriter::GetWrittenByteCount()
{
    return (size_t)_writeAddress - (size_t)_startAddress;
}