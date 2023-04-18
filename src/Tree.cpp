#include <cstdlib>
#include <cstring>
#include "huffman_simple.hpp"

huffman_simple::Tree::Tree(CodecIOState& state, Sorter& sorter)
{
    state.HuffmanTree = this;

    // Initialize tree
    _tree = (TreeNode*)malloc(sizeof(TreeNode) * (2 * sorter._presentSymbolCount) - 1);
    for (size_t i = 0; i < sorter._presentSymbolCount; ++i)
    {
        _tree[i].Symbol.Char = sorter._table[i].Char;
        _tree[i].Symbol.Freq = sorter._table[i].Freq;
        _tree[i].Flag = UNCHECKED;
        _tree[i].LeftIndex = 0;
        _tree[i].RightIndex = 0;
        _tree[i].BitCodeBitWidth = 0;
        _tree[i].BitCode = 0;
    }

    // Build tree
    _nodeCount = sorter._presentSymbolCount;
    if (sorter._presentSymbolCount != 1)
    {
        while (_nodeCount != (2 * sorter._presentSymbolCount) - 1)
        {
            // Extract 2 nodes with lowest frequency
            uint32_t minIndex1 = 0;
            uint32_t minIndex2 = 0;
            size_t minFreq;
            for (uint32_t i = 0, minFreq = (size_t)SIZE_MAX; i < _nodeCount; i++)
            {
                if (_tree[i].Flag == UNCHECKED && _tree[i].Symbol.Freq < minFreq)
                {
                    minIndex1 = i;
                    minFreq = _tree[i].Symbol.Freq;
                }
            }
            _tree[minIndex1].Flag = CHECKED;
            for (uint32_t i = 0, minFreq = (size_t)SIZE_MAX; i < _nodeCount; i++)
            {
                if (_tree[i].Flag == UNCHECKED && _tree[i].Symbol.Freq < minFreq)
                {
                    minIndex2 = i;
                    minFreq = _tree[i].Symbol.Freq;
                }
            }
            _tree[minIndex2].Flag = CHECKED;

            // Insert new parent node
            _tree[_nodeCount].Symbol.Char = NULL_SYMBOL;
            _tree[_nodeCount].Symbol.Freq = _tree[minIndex1].Symbol.Freq + _tree[minIndex2].Symbol.Freq;
            _tree[_nodeCount].Flag = UNCHECKED;
            _tree[_nodeCount].LeftIndex = minIndex1;
            _tree[_nodeCount].RightIndex = minIndex2;
            _nodeCount += 1;
        }
    }
    else
    {
        // Insert new parent node
        _tree[1].Symbol.Char = NULL_SYMBOL;
        _tree[1].Symbol.Freq = _tree[0].Symbol.Freq;
        _tree[1].Flag = CHECKED;
        _tree[1].LeftIndex = 0;
        _tree[1].RightIndex = 0;
        _nodeCount += 1;
    }

    // Assign codes to each node
    memset(&_symbolTreeIndex[0], 0, sizeof(uint32_t) * 256);
    AssignCodes(_nodeCount - 1, 0, 0);
}

huffman_simple::Tree::~Tree()
{
    free(_tree);
}

void huffman_simple::Tree::AssignCodes(uint32_t index, size_t bitCodeBitWidth, uint64_t bitCode)
{
    if (_tree[index].Symbol.Char == NULL_SYMBOL)
    {
        AssignCodes(_tree[index].LeftIndex, bitCodeBitWidth + 1, bitCode);
        AssignCodes(_tree[index].RightIndex, bitCodeBitWidth + 1, bitCode | (1 << bitCodeBitWidth));
    }
    else
    {
        _tree[index].BitCodeBitWidth = bitCodeBitWidth;
        _tree[index].BitCode = bitCode;
        _symbolTreeIndex[_tree[index].Symbol.Char] = index;
    }
}