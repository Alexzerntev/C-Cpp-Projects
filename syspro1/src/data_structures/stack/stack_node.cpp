#include "../../../include/data_structures/stack/stack_node.hpp"
#include "../../../include/data_structures/trie/trie_node.hpp"

StackNode::StackNode(TrieNode *trie_node, StackNode *up_node, StackNode *down_node)
{
    this->trie_node = trie_node;
    this->up_node = up_node;
    this->down_node = down_node;
}

StackNode::StackNode()
{
    this->trie_node = NULL;
    this->up_node = NULL;
    this->down_node = NULL;
}

StackNode::~StackNode()
{
}