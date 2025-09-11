#include "huffman_simple.hpp"

huffman_simple::BitWriter::BitWriter(const u8* const writeAddress)
{
    _startAddress = writeAddress;
    _writeAddress = (dword*)_startAddress;
    _wordBitCount = 0;
    _word = 0;
}

usize huffman_simple::BitWriter::WriteBits(const usize count, const qword bits)
{
    while (_wordBitCount >= 32)
    {
        _writeAddress[0] = _word & 0xffffffff;
        _writeAddress++;
        _wordBitCount -= 32;
        _word >>= 32;
    }

    _word |= bits << _wordBitCount;
    _wordBitCount += count;

    return (u8*)_writeAddress - _startAddress;
}

void huffman_simple::BitWriter::WriteBitsFromAddress(const usize byteCount, const u8* const address)
{
    const u8* const typedAddress = (const u8* const)address;
    for (usize i = 0; i < byteCount; ++i)
        WriteBits(8, (qword)typedAddress[i]);
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

usize huffman_simple::BitWriter::GetWrittenByteCount()
{
    return (usize)_writeAddress - (usize)_startAddress;
}