#include "huffman_simple.hpp"

huffman_simple::Analyzer::Analyzer(const CodecIOState& state)
{
    LogToConsole("Analyzing input data...");

    const u8* const data = (const u8* const)state.GetInputData();
    const usize dataByteSize = state.GetInputDataByteSize();
    
    // Initialize symbol table
    for (usize i = 0; i < 256; ++i)
    {
        _table[i].Char = (u16)i;
        _table[i].Freq = 0;
    }

    // Count symbols
    for (usize i = 0; i < dataByteSize; ++i)
        _table[data[i]].Freq++;

    // Store only present symbols
    _usedSymbolCount = 0;
    for (usize i = 0; i < 256; ++i)
    {
        if (_table[i].Freq != 0)
        {
            _table[_usedSymbolCount] = _table[i];
            _usedSymbolCount += 1;
        }
    }

    LogToConsole("    Success");
}