#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include "../error_handler/error_handler.h"
#include "../string/string_handler.h"
#include "../model/settings/settings.h"
#include "../data_structures/hashtable/hashtable.h"
#include "./user_interaction.h"

int validate_and_store(char *console_input[], int size, Settings *settings);
Hashtable *read_bitcoin_balances_file(char *file_name,
                                      Settings *settings,
                                      int *result_code,
                                      BitcoinTreeNode **all_nodes,
                                      int *counter);

int read_transactions_file(char *file_name,
                           Settings *settings,
                           Hashtable **receiver_hashtable,
                           Hashtable **sender_hashtable,
                           Hashtable *bitcoin_hashtable,
                           char *return_last_date,
                           char *return_last_time,
                           int *last_transactions_id);

BitcoinTreeNode **get_all_of_bitcoins(char *file_name, int *result_code);

int get_line_count(FILE *fp);

#endif