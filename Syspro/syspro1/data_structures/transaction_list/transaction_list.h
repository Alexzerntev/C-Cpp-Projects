#ifndef TRANSACTION_LIST_H
#define TRANSACTION_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include "../../error_handler/error_handler.h"
#include "../../string/string_handler.h"

typedef struct Transaction
{
    char *id;
    char *sender_id;
    char *receiver_id;
    double amount;
    char date[11];
    char time[6];
} Transaction;

typedef struct TransactionListNode
{
    struct TransactionListNode *next;
    struct TransactionListNode *previous;
    struct Transaction *transaction;
} TransactionListNode;

TransactionListNode *create_transaction_node(int id,
                                             char *sender_id,
                                             char *receiver_id,
                                             double amount,
                                             char *date,
                                             char *time);

Transaction *create_transaction(int id,
                                char *sender_id,
                                char *receiver_id,
                                double amount,
                                char *date,
                                char *time);

int add_transaction_node(TransactionListNode **start_node, TransactionListNode *to_be_added);

TransactionListNode *tokenize_and_create_transaction_list_node(char *transaction_data, char *last_date, char *last_time, int *result_code);
TransactionListNode *copy_transaction_node(TransactionListNode *node, int *result_code);

void destroy_transaction_nodes(TransactionListNode *transaction_list_node, int will_destroy_transaction);
void destroy_transaction(Transaction *transaction);

#endif