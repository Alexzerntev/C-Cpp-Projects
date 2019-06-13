#ifndef BITCOIN_TREE_H
#define BITCOIN_TREE_H

#include "../../string/string_handler.h"
#include "../transaction_list/transaction_list.h"
#include "errno.h"

typedef struct BitcoinTreeNode
{
    int id;
    double value; // Shows the remain value of this bitcoin
    int is_root;
    struct BitcoinTreeNode *next;
    struct BitcoinTreeNode *from_child;
    struct BitcoinTreeNode *to_child;
    struct TransactionListNode *transaction_node;
} BitcoinTreeNode;

BitcoinTreeNode *create_bitcoin_node(int id, double value, BitcoinTreeNode **all_nodes, int *counter);
int add_bitcoin_node(BitcoinTreeNode **start_node, int id, double value, BitcoinTreeNode **all_nodes, int *counter);
void add_bitcoin_transaction_node(BitcoinTreeNode *start_node, BitcoinTreeNode *bitcoin_transaction_node);
void destroy_bitcoin_nodes(BitcoinTreeNode *node);
void destroy_post_order(BitcoinTreeNode *node);

#endif