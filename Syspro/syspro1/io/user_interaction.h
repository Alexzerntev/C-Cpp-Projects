#ifndef USER_INTERACTION_H
#define USER_INTERACTION_H

#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "../data_structures/transaction_list/transaction_list.h"
#include "../data_structures/hashtable/hashtable.h"
#include "../data_structures/bitcoin_tree/bitcoin_tree.h"
#include "../error_handler/error_handler.h"
#include "../model/settings/settings.h"
#include "../date_time/date_time.h"

void interact(Hashtable *bitcoin_hashtable,
              Hashtable *receiver_hashtable,
              Hashtable *sender_hashtable,
              Settings *settings,
              BitcoinTreeNode **all_nodes,
              int counter,
              int last_transaction_id,
              char *last_date,
              char *last_time);

int request_transaction(char *line,
                        Hashtable *bitcoin_hashtable,
                        Hashtable *sender_hashtable,
                        Hashtable *receiver_hashtable);

int request_transactions(char *line,
                         Hashtable *bitcoin_hashtable,
                         Hashtable *sender_hashtable,
                         Hashtable *receiver_hashtable);

int request_transactions_from_file(char *line,
                                   Hashtable *bitcoin_hashtable,
                                   Hashtable *sender_hashtable,
                                   Hashtable *receiver_hashtable,
                                   Settings *settings);

int find_earnings(char *line, Hashtable *bitcoin_hashtable);

int find_payments(char *line, Hashtable *sender_hashtable);

int wallet_status(char *line, Hashtable *bitcoin_hashtable);

int bitcoin_status(char *line, BitcoinTreeNode **all_nodes, int counter);

int trace_coin(char *line, BitcoinTreeNode **all_nodes, int counter);

int word_count(const char *line, int length);

void in_order_visit(BitcoinTreeNode *node, int *count, int *leaf_count);

void in_order_visit_history(BitcoinTreeNode *node);

void get_datetime_tokens(char *input_string, char *time1, char *time2, char *date1, char *date2);

void set_generator_value(int value);

int get_next_generator_value();

#endif