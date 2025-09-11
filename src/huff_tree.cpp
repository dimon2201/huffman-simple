#include <cstdlib>
#include <cstring>
#include "huffman_simple.hpp"

huffman_simple::Tree::Tree(CodecIOState& state, Analyzer& analyzer)
{
    LogToConsole("Constructing tree...");

    state.SetTree(this);

    const usize usedSymbolCount = analyzer.GetUsedSymbolCount();
    const Symbol* table = analyzer.GetTable();

    // Initialize tree
    _tree = (TreeNode*)malloc(sizeof(TreeNode) * 2 * usedSymbolCount - 1);
    for (usize i = 0; i < usedSymbolCount; ++i)
    {
        _tree[i].Symbol.Char = table[i].Char;
        _tree[i].Symbol.Freq = table[i].Freq;
        _tree[i].Flag = K_UNCHECKED;
        _tree[i].LeftIndex = 0;
        _tree[i].RightIndex = 0;
        _tree[i].BitCode = 0;
        _tree[i].BitCodeBitSize = 0;
    }

    // Build tree
    _nodeCount = usedSymbolCount;
    if (usedSymbolCount != 1)
    {
        while (_nodeCount != 2 * usedSymbolCount - 1)
        {
            // Extract 2 nodes with lowest frequency
            u32 minIndex1 = 0;
            u32 minIndex2 = 0;
            usize minFreq = 0;
            for (usize i = 0, minFreq = K_USIZE_MAX; i < _nodeCount; i++)
            {
                if (_tree[i].Flag == K_UNCHECKED && _tree[i].Symbol.Freq < minFreq)
                {
                    minIndex1 = (u32)i;
                    minFreq = _tree[i].Symbol.Freq;
                }
            }
            _tree[minIndex1].Flag = K_CHECKED;

            for (usize i = 0, minFreq = SIZE_MAX; i < _nodeCount; i++)
            {
                if (_tree[i].Flag == K_UNCHECKED && _tree[i].Symbol.Freq < minFreq)
                {
                    minIndex2 = (u32)i;
                    minFreq = _tree[i].Symbol.Freq;
                }
            }
            _tree[minIndex2].Flag = K_CHECKED;

            // Insert new parent node
            _tree[_nodeCount].Symbol.Char = K_NULL_SYMBOL;
            _tree[_nodeCount].Symbol.Freq = _tree[minIndex1].Symbol.Freq + _tree[minIndex2].Symbol.Freq;
            _tree[_nodeCount].Flag = K_UNCHECKED;
            _tree[_nodeCount].LeftIndex = minIndex1;
            _tree[_nodeCount].RightIndex = minIndex2;
            _nodeCount += 1;
        }
    }
    else
    {
        // Insert new parent node
        _tree[1].Symbol.Char = K_NULL_SYMBOL;
        _tree[1].Symbol.Freq = _tree[0].Symbol.Freq;
        _tree[1].Flag = K_CHECKED;
        _tree[1].LeftIndex = 0;
        _tree[1].RightIndex = 0;
        _nodeCount += 1;
    }

    // Assign codes to each node
    memset(&_symbolTreeIndex[0], 0, sizeof(u32) * 256);
    AssignCodes((u32)_nodeCount - 1, 0, 0);

    LogToConsole("    Success");
}

huffman_simple::Tree::~Tree()
{
    free(_tree);
}

void huffman_simple::Tree::AssignCodes(const u32 index, const usize bitCodeBitWidth, const qword bitCode)
{
    if (_tree[index].Symbol.Char == K_NULL_SYMBOL)
    {
        AssignCodes(_tree[index].LeftIndex, bitCodeBitWidth + 1, bitCode);
        AssignCodes(_tree[index].RightIndex, bitCodeBitWidth + 1, bitCode | (1ull << bitCodeBitWidth));
    }
    else
    {
        _tree[index].BitCode = bitCode;
        _tree[index].BitCodeBitSize = bitCodeBitWidth;
        _symbolTreeIndex[_tree[index].Symbol.Char] = index;
    }
}