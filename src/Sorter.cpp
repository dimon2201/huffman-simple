#include "huffman_simple.hpp"

huffman_simple::Sorter::Sorter(CodecIOState& state)
{
    const uint8_t* data = (const uint8_t*)state.InputData;
    const size_t byteWidth = state.InputByteWidth;
    
    // Initialize symbol table
    for (size_t i = 0; i < 256; ++i)
    {
        _table[i].Char = (uint16_t)i;
        _table[i].Freq = 0;
    }

    // Count symbols
    for (size_t i = 0; i < byteWidth; ++i) { _table[data[i]].Freq += 1; }

    // Store only present symbols
    _presentSymbolCount = 0;
    for (size_t i = 0; i < 256; ++i)
    {
        if (_table[i].Freq != 0)
        {
            _table[_presentSymbolCount] = _table[i];
            _presentSymbolCount += 1;
        }
    }

    // Sort symbols
    size_t swapIndex = 0;
    for (size_t i = 0; i < _presentSymbolCount - 1; ++i)
    {
        for (size_t j = 0; j < _presentSymbolCount - i - 1; ++j)
        {
            if (_table[j].Freq > _table[j + 1].Freq)
            {
                Symbol temp = _table[j];
                _table[j] = _table[j + 1];
                _table[j + 1] = temp;
            }
        }
    }
}

huffman_simple::Sorter::~Sorter()
{

}