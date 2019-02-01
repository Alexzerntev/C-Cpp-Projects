#include "../../../include/data_structures/stack/stack.hpp"

Stack::Stack()
{
    this->top = NULL;
}
Stack::~Stack()
{
}

void Stack::print()
{
    StackNode *current_node = this->top;
    while (current_node->down_node != NULL)
    {
        current_node = current_node->down_node;
    }
    while (true)
    {
        printf("%c", current_node->trie_node->get_content());
        if (current_node->up_node == NULL)
        {
            break;
        }
        current_node = current_node->up_node;
    }
    if (current_node->trie_node != NULL)
    {
        if (current_node->trie_node->posting_list != NULL)
        {
            printf(" %d", current_node->trie_node->posting_list->get_count());
        }
    }
    printf("\n");
}

TrieNode *Stack::pop()
{
    TrieNode *trie_node_to_return = top->trie_node;
    StackNode *temp = this->top;
    this->top = this->top->down_node;
    if (this->top != NULL)
    {
        this->top->up_node = NULL;
    }
    delete (temp);
    return trie_node_to_return;
}

void Stack::push(TrieNode *trie_node)
{
    if (this->top == NULL)
    {
        this->top = new StackNode(trie_node, NULL, NULL);
        return;
    }
    StackNode *temp = new StackNode(trie_node, NULL, this->top);
    this->top->up_node = temp;
    this->top = temp;
}

bool Stack::is_empty()
{
    return this->top == NULL;
}