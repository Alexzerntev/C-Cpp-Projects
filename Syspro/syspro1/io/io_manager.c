#include "./io_manager.h"

int validate_and_store(char **console_input, int size, Settings *settings)
{
    int result_code = SUCCESS;
    if (size != 13)
    {
        return WRONG_INITIALIZATION_ERROR;
    }

    int i;
    for (i = 1; i < size; i++)
    {
        if (strcmp(console_input[i], "-a") == 0)
        {
            i++;
            settings->bitcoin_balances_file = copy_string(console_input[i], &result_code);

            if (result_code != SUCCESS)
            {
                print_error(result_code, "Bitcoin balance file name");
                return WRONG_INITIALIZATION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-t") == 0)
        {
            i++;
            settings->transactions_file = copy_string(console_input[i], &result_code);

            if (result_code != SUCCESS)
            {
                print_error(result_code, "Transactions file name");
                return WRONG_INITIALIZATION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-v") == 0)
        {
            i++;
            settings->bitcoin_value = atof(console_input[i]);

            if (settings->bitcoin_value == 0.0)
            {
                print_error(FLOAT_CONVERTION_ERROR, "Bitcoin value");
                return WRONG_INITIALIZATION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-h1") == 0)
        {
            i++;
            settings->sender_hashtable_entries = atof(console_input[i]);

            if (settings->sender_hashtable_entries == 0.0)
            {
                print_error(FLOAT_CONVERTION_ERROR, "Sender hashtable");
                return WRONG_INITIALIZATION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-h2") == 0)
        {
            i++;
            settings->reciever_hashtable_entries = atof(console_input[i]);

            if (settings->reciever_hashtable_entries == 0.0)
            {
                print_error(FLOAT_CONVERTION_ERROR, "Sender hashtable");
                return WRONG_INITIALIZATION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-b") == 0)
        {
            i++;
            settings->bucket_size = atoi(console_input[i]);

            if (settings->bucket_size == 0)
            {
                print_error(INT_CONVERTION_ERROR, "Bucket size");
                return WRONG_INITIALIZATION_ERROR;
            }

            continue;
        }
        else
        {
            return WRONG_INITIALIZATION_ERROR;
        }
    }

    return SUCCESS;
}

Hashtable *read_bitcoin_balances_file(char *file_name,
                                      Settings *settings,
                                      int *result_code,
                                      BitcoinTreeNode **all_nodes,
                                      int *counter)
{
    int line_count = 0, hashtable_size = 0, entries_per_bucket = 0;
    char *line = NULL;
    size_t length = 0;
    FILE *fp = NULL;
    User *user = NULL;
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        print_error(READ_FILE_ERROR, "Read bitcoin balances");
        fclose(fp);
        return NULL;
    }

    line_count = get_line_count(fp);
    hashtable_size = get_bucket_count(line_count, settings->bucket_size, &entries_per_bucket);

    Hashtable *bitcoin_hashtable = create_hashtable(hashtable_size, entries_per_bucket, result_code);
    if (*result_code != SUCCESS)
    {
        fclose(fp);
        return NULL;
    }

    bitcoin_hashtable->size = hashtable_size;

    while (getline(&line, &length, fp) != EOF)
    {
        user = create_bitcoin_user(line, settings->bitcoin_value, all_nodes, counter);
        if (user == NULL)
        {
            return NULL;
        }
        add_user_to_bitcoin_hashtable(bitcoin_hashtable, user);
    }

    free(line);
    fclose(fp);
    return bitcoin_hashtable;
}

BitcoinTreeNode **get_all_of_bitcoins(char *file_name, int *result_code)
{
    FILE *file;
    int word_count = 1;
    int line_count = 1;
    char c;
    file = fopen(file_name, "r");
    if (file == NULL)
        printf("file not found\n");
    while ((c = fgetc(file)) != EOF)
    {
        if (c == ' ')
        {
            word_count++;
        }
        if (c == '\n')
        {
            word_count++;
            line_count++;
        }
    }
    fclose(file);

    int i;
    BitcoinTreeNode **all_bitcoins = (BitcoinTreeNode **)malloc(sizeof(BitcoinTreeNode *) * (word_count - line_count));

    if (all_bitcoins == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "All bitcoins");
        return NULL;
    }

    for (i = 0; i < (word_count - line_count); i++)
    {
        all_bitcoins[i] = NULL;
    }

    *result_code = SUCCESS;

    return all_bitcoins;
}

int read_transactions_file(char *file_name,
                           Settings *settings,
                           Hashtable **receiver_hashtable,
                           Hashtable **sender_hashtable,
                           Hashtable *bitcoin_hashtable,
                           char *return_last_date,
                           char *return_last_time,
                           int *last_transaction_id)
{
    unsigned int entries_per_bucket;
    int result_code;
    char *line = NULL;
    size_t length = 0;
    FILE *fp;
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        print_error(READ_FILE_ERROR, "Read bitcoin balances");
        return READ_FILE_ERROR;
    }

    entries_per_bucket = get_entries_per_bucket(settings->bucket_size);
    *receiver_hashtable = create_hashtable(settings->reciever_hashtable_entries, entries_per_bucket, &result_code);
    *sender_hashtable = create_hashtable(settings->sender_hashtable_entries, entries_per_bucket, &result_code);
    TransactionListNode *transaction;
    char last_date[11] = "11-11-1111";
    char last_time[6] = "11:11";
    while (getline(&line, &length, fp) != EOF)
    {
        if (word_count(line, strlen(line)) != 6)
        {
            char *string_with_id = add_id_to_transaction(line, get_next_generator_value(), &result_code);
            remove_line_break(string_with_id);
            remove_semicolon(string_with_id);

            transaction = tokenize_and_create_transaction_list_node(string_with_id, last_date, last_time, &result_code);
            if (result_code != SUCCESS)
            {
                destroy_string(string_with_id);
                free(line);
                fclose(fp);
                return result_code;
            }

            result_code = execute_transaction(bitcoin_hashtable, transaction);
            if (result_code != SUCCESS)
            {
                destroy_string(string_with_id);
                continue;
            }

            result_code = add_to_transaction_hashtable(*receiver_hashtable,
                                                       *sender_hashtable,
                                                       transaction);
            if (result_code != SUCCESS)
            {
                destroy_string(string_with_id);
                free(line);
                fclose(fp);
                return result_code;
            }
            destroy_string(string_with_id);
            continue;
        }

        transaction = tokenize_and_create_transaction_list_node(line, last_date, last_time, &result_code);
        if (result_code != SUCCESS)
        {
            free(line);
            fclose(fp);
            return result_code;
        }

        result_code = execute_transaction(bitcoin_hashtable, transaction);
        if (result_code != SUCCESS)
        {
            destroy_transaction_nodes(transaction, 1);
            continue;
        }

        result_code = add_to_transaction_hashtable(*receiver_hashtable,
                                                   *sender_hashtable,
                                                   transaction);
        if (result_code != SUCCESS)
        {
            free(line);
            fclose(fp);
            return result_code;
        }
    }

    free(line);
    fclose(fp);
    strcpy(return_last_date, last_date);
    strcpy(return_last_time, last_time);

    *last_transaction_id = atoi(transaction->transaction->id);
    return SUCCESS;
}

int get_line_count(FILE *fp)
{
    // int count = -1; //because of the last blank line
    int count = 1;
    int ch;
    do
    {
        ch = fgetc(fp);
        if (ch == '\n')
            count++;
    } while (ch != EOF);

    rewind(fp);
    return count;
}
