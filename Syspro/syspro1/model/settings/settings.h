
#ifndef SETTINGS_H
#define SETTINGS_H

#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "../../error_handler/error_handler.h"

typedef struct Settings
{
    float bitcoin_value;
    int sender_hashtable_entries;
    int reciever_hashtable_entries;
    int bucket_size;
    char *bitcoin_balances_file;
    char *transactions_file;
} Settings;

void destroy_settings(Settings *settings, int is_partial);

#endif