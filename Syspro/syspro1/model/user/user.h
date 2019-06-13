#ifndef USER_H
#define USER_H

#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "../../error_handler/error_handler.h"
#include "../../data_structures/transaction_list/transaction_list.h"
#include "../../data_structures/bitcoin_tree/bitcoin_tree.h"
#include "../../string/string_handler.h"

// ========================== User ===============================

typedef struct User
{
    char *id;
    struct BitcoinTreeNode *bitcoins;
    double current_amount;
    struct TransactionListNode *transction_list;
} User;

User *create_bitcoin_user(char *user_entry, double bitcoin_value, BitcoinTreeNode **all_nodes, int *counter);
User *create_transaction_user(char *user_id);

void destroy_user(User *user, int will_destroy_transaction);

// int add_bitcoin(User *user, int bitcoin_id, double bitcoin_value);

#endif