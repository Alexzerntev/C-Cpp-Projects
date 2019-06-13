#include "./bitcoin_tree.h"

BitcoinTreeNode *create_bitcoin_node(int id, double value, BitcoinTreeNode **all_nodes, int *counter)
{
    BitcoinTreeNode *node = (BitcoinTreeNode *)malloc(sizeof(BitcoinTreeNode));

    if (node == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Bitcoin tree node");
        return NULL;
    }

    if (all_nodes != NULL)
    {
        all_nodes[*counter] = node;
        (*counter)++;
    }

    node->from_child = NULL;
    node->to_child = NULL;
    node->next = NULL;
    node->value = value;
    node->id = id;
    node->is_root = 0;
    node->transaction_node = NULL;

    return node;
}

int add_bitcoin_node(BitcoinTreeNode **start_node, int id, double value, BitcoinTreeNode **all_nodes, int *counter)
{
    BitcoinTreeNode *temp = create_bitcoin_node(id, value, all_nodes, counter);

    if (temp == NULL)
    {
        return MEMORY_ALLOCATION_ERROR;
    }

    temp->next = *start_node;
    *start_node = temp;

    return SUCCESS;
}

// Is next is to add to a bitcoin node from transaction or not
void add_bitcoin_transaction_node(BitcoinTreeNode *start_node, BitcoinTreeNode *bitcoin_transaction_node)
{
    BitcoinTreeNode *temp;
    temp = start_node;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = bitcoin_transaction_node;

    return;
}

void destroy_bitcoin_nodes(BitcoinTreeNode *node)
{
    BitcoinTreeNode *temp;
    while (node != NULL)
    {
        temp = node;
        node = node->next;
        if (temp->is_root == 1)
        {
            destroy_post_order(temp);
        }
    }
}

void destroy_post_order(BitcoinTreeNode *node)
{
    if (node != NULL)
    {
        destroy_post_order(node->from_child);
        destroy_post_order(node->to_child);
        free(node);
    }
}