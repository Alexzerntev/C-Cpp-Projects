#include "./user_interaction.h"

static int last_id = 0;
static char last_date[11] = "01-01-1900";
static char last_time[6] = "00:00";

void interact(Hashtable *bitcoin_hashtable,
              Hashtable *receiver_hashtable,
              Hashtable *sender_hashtable,
              Settings *settings,
              BitcoinTreeNode **all_nodes,
              int all_bitcoins_counter,
              int last_transaction_id,
              char *last_date,
              char *last_time)
{
    last_id = last_transaction_id;
    size_t length;
    char *line = NULL;
    int result_code;

    while (1)
    {
        printf("\nGive me a command to execute\n");
        getline(&line, &length, stdin);
        if (strstr(line, "/requestTransaction") != NULL && strstr(line, "/requestTransactions") == NULL)
        {
            result_code = request_transaction(line,
                                              bitcoin_hashtable,
                                              receiver_hashtable,
                                              sender_hashtable);
            if (result_code != SUCCESS)
            {
                print_error(result_code, "Request transaction");
            }

            continue;
        }
        if (strstr(line, "/requestTransactions") != NULL)
        {
            if (word_count(line, strlen(line)) >= 3)
            {
                result_code = request_transactions(line,
                                                   bitcoin_hashtable,
                                                   receiver_hashtable,
                                                   sender_hashtable);
                if (result_code != SUCCESS)
                {
                    print_error(result_code, "Request transactions");
                }
            }
            else
            {
                result_code = request_transactions_from_file(line,
                                                             bitcoin_hashtable,
                                                             receiver_hashtable,
                                                             sender_hashtable,
                                                             settings);
            }
            continue;
        }
        if (strstr(line, "/findEarnings") != NULL)
        {
            result_code = find_earnings(line, receiver_hashtable);
            if (result_code != SUCCESS)
            {
                print_error(result_code, "Find Earnings");
            }
            continue;
        }
        if (strstr(line, "/findPayments") != NULL)
        {
            result_code = find_payments(line, sender_hashtable);
            if (result_code != SUCCESS)
            {
                print_error(result_code, "Find Payments");
            }
            continue;
        }
        if (strstr(line, "/walletStatus") != NULL)
        {
            result_code = wallet_status(line, bitcoin_hashtable);
            if (result_code != SUCCESS)
            {
                print_error(result_code, "Wallet Status");
            }
            continue;
        }
        if (strstr(line, "/bitCoinStatus") != NULL)
        {
            result_code = bitcoin_status(line, all_nodes, all_bitcoins_counter);
            if (result_code != SUCCESS)
            {
                print_error(result_code, "Bitcoin Status");
            }
            continue;
        }
        if (strstr(line, "/traceCoin") != NULL)
        {
            result_code = trace_coin(line, all_nodes, all_bitcoins_counter);
            if (result_code != SUCCESS)
            {
                print_error(result_code, "Trace Coin");
            }
            continue;
        }
        if (strstr(line, "/exit") != NULL)
        {
            free(line);
            return;
        }
        printf("\nWrong Input, try again\n");
    }
}

int request_transaction(char *line,
                        Hashtable *bitcoin_hashtable,
                        Hashtable *receiver_hashtable,
                        Hashtable *sender_hashtable)
{

    int result_code;
    TransactionListNode *transaction;
    char *without_command = line;
    while (*without_command != 0 && *(without_command++) != ' ')
    {
    }

    remove_line_break(without_command);
    if (word_count(without_command, strlen(without_command)) == 3)
    {
        char *string_without_date = add_id_to_transaction(without_command, get_next_generator_value(), &result_code);
        char *final_string = add_date_time_to_transaction(string_without_date, &result_code);
        transaction = tokenize_and_create_transaction_list_node(final_string, last_date, last_time, &result_code);
        destroy_string(string_without_date);
        destroy_string(final_string);
    }
    else if (word_count(without_command, strlen(without_command)) == 4)
    {
        char *final_string = add_id_to_transaction(without_command, get_next_generator_value(), &result_code);
        transaction = tokenize_and_create_transaction_list_node(final_string, last_date, last_time, &result_code);
        destroy_string(final_string);
    }
    else
    {
        return WRONG_INPUT;
    }

    if (result_code != SUCCESS)
    {
        return result_code;
    }

    result_code = execute_transaction(bitcoin_hashtable, transaction);
    if (result_code != SUCCESS)
    {
        return result_code;
    }

    result_code = add_to_transaction_hashtable(receiver_hashtable,
                                               sender_hashtable,
                                               transaction);
    printf("Transaction %f from %s to %s has been executed sucesfully!\n", transaction->transaction->amount, transaction->transaction->sender_id, transaction->transaction->receiver_id);
    return SUCCESS;
}

int request_transactions(char *line,
                         Hashtable *bitcoin_hashtable,
                         Hashtable *receiver_hashtable,
                         Hashtable *sender_hashtable)
{
    int result_code;
    char *temp_string = copy_string(line, &result_code);
    remove_line_break(temp_string);
    char *without_command = temp_string;
    while (*without_command != 0 && *(without_command++) != ' ')
    {
    }
    int is_first = 1;

    char *token = strtok(without_command, ";");

    TransactionListNode *transaction;
    while (token != NULL)
    {
        if (is_first == 1)
        {
            if (word_count(token, strlen(token)) == 3)
            {
                char *string_without_date = add_id_to_transaction(token, get_next_generator_value(), &result_code);
                char *final_string = add_date_time_to_transaction(string_without_date, &result_code);
                transaction = tokenize_and_create_transaction_list_node(final_string, last_date, last_time, &result_code);
                destroy_string(string_without_date);
                destroy_string(final_string);
            }
            else if (word_count(token, strlen(token)) == 5)
            {
                char *final_string = add_id_to_transaction(token, get_next_generator_value(), &result_code);
                transaction = tokenize_and_create_transaction_list_node(final_string, last_date, last_time, &result_code);
                destroy_string(final_string);
            }
            else
            {
                return WRONG_INPUT;
            }

            result_code = execute_transaction(bitcoin_hashtable, transaction);
            if (result_code != SUCCESS)
            {
                return result_code;
            }

            result_code = add_to_transaction_hashtable(receiver_hashtable,
                                                       sender_hashtable,
                                                       transaction);

            printf("Transaction %f from %s to %s has been executed sucesfully!\n", transaction->transaction->amount, transaction->transaction->sender_id, transaction->transaction->receiver_id);
            token = strtok(token + strlen(token) + 1, ";");
            is_first = 0;
            continue;
        }
        remove_line_break(token);
        if (word_count(token, strlen(token)) == 3)
        {
            char *string_without_date = add_id_to_transaction(token, get_next_generator_value(), &result_code);
            char *final_string = add_date_time_to_transaction(string_without_date, &result_code);
            transaction = tokenize_and_create_transaction_list_node(final_string, last_date, last_time, &result_code);
            destroy_string(string_without_date);
            destroy_string(final_string);
        }
        else if (word_count(token, strlen(token)) == 4)
        {
            char *final_string = add_id_to_transaction(token, get_next_generator_value(), &result_code);
            transaction = tokenize_and_create_transaction_list_node(final_string, last_date, last_time, &result_code);
            destroy_string(final_string);
        }
        else
        {
            return WRONG_INPUT;
        }

        result_code = execute_transaction(bitcoin_hashtable, transaction);
        if (result_code != SUCCESS)
        {
            destroy_transaction_nodes(transaction, 1);
            return result_code;
        }

        result_code = add_to_transaction_hashtable(receiver_hashtable,
                                                   sender_hashtable,
                                                   transaction);
        if (result_code != SUCCESS)
        {
            return result_code;
        }

        printf("Transaction %f from %s to %s has been executed sucesfully!\n", transaction->transaction->amount, transaction->transaction->sender_id, transaction->transaction->receiver_id);
        token = strtok(token + strlen(token) + 1, ";");
    }

    destroy_string(temp_string);
    return SUCCESS;
}

int request_transactions_from_file(char *input_line,
                                   Hashtable *bitcoin_hashtable,
                                   Hashtable *receiver_hashtable,
                                   Hashtable *sender_hashtable,
                                   Settings *settings)
{
    char *token;
    int result_code;

    char *temp_string = copy_string(input_line, &result_code);
    remove_line_break(temp_string);
    token = strtok(temp_string, " ");
    token = strtok(NULL, " ");

    char *line = NULL;
    size_t length = 0;
    FILE *fp;
    fp = fopen(token, "r");
    if (fp == NULL)
    {
        print_error(READ_FILE_ERROR, "Read bitcoin balances");
        return READ_FILE_ERROR;
    }

    TransactionListNode *transaction;
    while (getline(&line, &length, fp) != EOF)
    {
        remove_line_break(line);
        if (word_count(line, strlen(line)) == 3)
        {
            char *string_without_date = add_id_to_transaction(line, get_next_generator_value(), &result_code);
            char *final_string = add_date_time_to_transaction(string_without_date, &result_code);
            transaction = tokenize_and_create_transaction_list_node(final_string, last_date, last_time, &result_code);
            destroy_string(string_without_date);
            destroy_string(final_string);
        }
        else if (word_count(line, strlen(line)) == 4)
        {
            char *final_string = add_id_to_transaction(line, get_next_generator_value(), &result_code);
            transaction = tokenize_and_create_transaction_list_node(final_string, last_date, last_time, &result_code);
            destroy_string(final_string);
        }
        else
        {
            return WRONG_INPUT;
        }

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

        result_code = add_to_transaction_hashtable(receiver_hashtable,
                                                   sender_hashtable,
                                                   transaction);
        if (result_code != SUCCESS)
        {
            free(line);
            fclose(fp);
            return result_code;
        }
        printf("Transaction %f from %s to %s has been executed sucesfully!\n", transaction->transaction->amount, transaction->transaction->sender_id, transaction->transaction->receiver_id);
    }

    free(line);
    fclose(fp);

    destroy_string(temp_string);
    return SUCCESS;
}

int find_earnings(char *line, Hashtable *receiver_hashtable)
{
    int result_code;

    char *time1 = create_string(5, &result_code);
    char *time2 = create_string(5, &result_code);
    char *date1 = create_string(10, &result_code);
    char *date2 = create_string(11, &result_code);

    char *token;
    double balance = 0;
    char *temp_string = copy_string(line, &result_code);
    token = strtok(temp_string, " ");
    token = strtok(NULL, " ");

    remove_line_break(token);
    User *user = find_user(receiver_hashtable, token, &result_code);
    if (result_code != SUCCESS)
    {
        printf("User not found");
        return SUCCESS;
    }
    TransactionListNode *current_transaction = user->transction_list;
    // case has datetime parameters
    if (word_count(line, strlen(line)) > 2)
    {
        get_datetime_tokens(line, time1, time2, date1, date2);

        printf("User's transactions are :\n");

        // case only date
        if (strcmp(time1, "-----") == 0)
        {
            strcpy(time1, "00:00");
            strcpy(time2, "23:59");
            while (current_transaction != NULL)
            {
                if (is_between_date_time(date1, time1, current_transaction->transaction->date, current_transaction->transaction->time, date2, time2) == 1)
                {
                    printf("%s %s %s %f %s %s\n",
                           current_transaction->transaction->id,
                           current_transaction->transaction->sender_id,
                           current_transaction->transaction->receiver_id,
                           current_transaction->transaction->amount,
                           current_transaction->transaction->date,
                           current_transaction->transaction->time);
                    balance = balance + current_transaction->transaction->amount;
                }

                current_transaction = current_transaction->next;
            }
        }
        // case only time
        else if (strcmp(date1, "----------") == 0)
        {
            char *current_date = get_now_date(&result_code);
            while (current_transaction != NULL)
            {
                if (is_between_date_time(current_date, time1, current_transaction->transaction->date, current_transaction->transaction->time, current_date, time2) == 1)
                {
                    printf("%s %s %s %f %s %s\n",
                           current_transaction->transaction->id,
                           current_transaction->transaction->sender_id,
                           current_transaction->transaction->receiver_id,
                           current_transaction->transaction->amount,
                           current_transaction->transaction->date,
                           current_transaction->transaction->time);
                    balance = balance + current_transaction->transaction->amount;
                }

                current_transaction = current_transaction->next;
            }
            destroy_string(current_date);
        }
        // case both
        else
        {
            while (current_transaction != NULL)
            {
                if (is_between_date_time(date1, time1, current_transaction->transaction->date, current_transaction->transaction->time, date2, time2) == 1)
                {
                    printf("%s %s %s %f %s %s\n",
                           current_transaction->transaction->id,
                           current_transaction->transaction->sender_id,
                           current_transaction->transaction->receiver_id,
                           current_transaction->transaction->amount,
                           current_transaction->transaction->date,
                           current_transaction->transaction->time);
                    balance = balance + current_transaction->transaction->amount;
                }

                current_transaction = current_transaction->next;
            }
        }
    }
    // case no datetime parameters
    else
    {
        printf("User's transactions are :\n");
        while (current_transaction != NULL)
        {
            printf("%s %s %s %f %s %s\n",
                   current_transaction->transaction->id,
                   current_transaction->transaction->sender_id,
                   current_transaction->transaction->receiver_id,
                   current_transaction->transaction->amount,
                   current_transaction->transaction->date,
                   current_transaction->transaction->time);

            balance = balance + current_transaction->transaction->amount;
            current_transaction = current_transaction->next;
        }
    }

    destroy_string(date1);
    destroy_string(date2);
    destroy_string(time1);
    destroy_string(time2);
    destroy_string(temp_string);

    printf("The total transferred amount is : %f\n", balance);

    return SUCCESS;
}

int find_payments(char *line, Hashtable *sender_hashtable)
{
    int result_code;

    char *time1 = create_string(5, &result_code);
    char *time2 = create_string(5, &result_code);
    char *date1 = create_string(10, &result_code);
    char *date2 = create_string(11, &result_code);

    char *token;
    double balance = 0;
    char *temp_string = copy_string(line, &result_code);
    token = strtok(temp_string, " ");
    token = strtok(NULL, " ");
    remove_line_break(token);
    User *user = find_user(sender_hashtable, token, &result_code);
    if (result_code != SUCCESS)
    {
        printf("User not found");
        return SUCCESS;
    }

    TransactionListNode *current_transaction = user->transction_list;
    // case has datetime parameters
    if (word_count(line, strlen(line)) > 2)
    {
        get_datetime_tokens(line, time1, time2, date1, date2);

        printf("User's transactions are :\n");

        // case only date
        if (strcmp(time1, "-----") == 0)
        {
            strcpy(time1, "00:00");
            strcpy(time2, "23:59");
            while (current_transaction != NULL)
            {
                if (is_between_date_time(date1, time1, current_transaction->transaction->date, current_transaction->transaction->time, date2, time2) == 1)
                {
                    printf("%s %s %s %f %s %s\n",
                           current_transaction->transaction->id,
                           current_transaction->transaction->sender_id,
                           current_transaction->transaction->receiver_id,
                           current_transaction->transaction->amount,
                           current_transaction->transaction->date,
                           current_transaction->transaction->time);
                    balance = balance + current_transaction->transaction->amount;
                }

                current_transaction = current_transaction->next;
            }
        }
        // case only time
        else if (strcmp(date1, "----------") == 0)
        {
            char *current_date = get_now_date(&result_code);
            while (current_transaction != NULL)
            {
                if (is_between_date_time(current_date, time1, current_transaction->transaction->date, current_transaction->transaction->time, current_date, time2) == 1)
                {
                    printf("%s %s %s %f %s %s\n",
                           current_transaction->transaction->id,
                           current_transaction->transaction->sender_id,
                           current_transaction->transaction->receiver_id,
                           current_transaction->transaction->amount,
                           current_transaction->transaction->date,
                           current_transaction->transaction->time);
                    balance = balance + current_transaction->transaction->amount;
                }

                current_transaction = current_transaction->next;
            }
            destroy_string(current_date);
        }
        // case both
        else
        {
            while (current_transaction != NULL)
            {
                if (is_between_date_time(date1, time1, current_transaction->transaction->date, current_transaction->transaction->time, date2, time2) == 1)
                {
                    printf("%s %s %s %f %s %s\n",
                           current_transaction->transaction->id,
                           current_transaction->transaction->sender_id,
                           current_transaction->transaction->receiver_id,
                           current_transaction->transaction->amount,
                           current_transaction->transaction->date,
                           current_transaction->transaction->time);
                    balance = balance + current_transaction->transaction->amount;
                }

                current_transaction = current_transaction->next;
            }
        }
    }
    // case no datetime parameters
    else
    {
        printf("User's transactions are :\n");
        while (current_transaction != NULL)
        {
            printf("%s %s %s %f %s %s\n",
                   current_transaction->transaction->id,
                   current_transaction->transaction->sender_id,
                   current_transaction->transaction->receiver_id,
                   current_transaction->transaction->amount,
                   current_transaction->transaction->date,
                   current_transaction->transaction->time);

            balance = balance + current_transaction->transaction->amount;
            current_transaction = current_transaction->next;
        }
    }

    destroy_string(date1);
    destroy_string(date2);
    destroy_string(time1);
    destroy_string(time2);
    destroy_string(temp_string);

    printf("The total transferred amount is : %f\n", balance);

    return SUCCESS;
}

int wallet_status(char *line, Hashtable *bitcoin_hashtable)
{
    char *token;
    int result_code;
    token = strtok(line, " ");
    token = strtok(NULL, " ");
    remove_line_break(token);
    User *user = find_user(bitcoin_hashtable, token, &result_code);

    if (result_code != SUCCESS)
    {
        printf("User not found");
        return SUCCESS;
    }

    printf("The current balance is : %f\n", user->current_amount);

    return SUCCESS;
}

int bitcoin_status(char *line, BitcoinTreeNode **all_nodes, int counter)
{
    char *token;
    int i, nodes_count = 0, leaf_count = 0;
    token = strtok(line, " ");
    token = strtok(NULL, " ");
    int bitcoin_id = atoi(token);

    BitcoinTreeNode *current_node;

    for (i = 0; i < counter; i++)
    {
        if (all_nodes[i]->id == bitcoin_id)
        {
            printf("The initial value of bitcoin %d is : %f\n", bitcoin_id, all_nodes[i]->value);
            nodes_count = 0;
            leaf_count = 0;
            in_order_visit(all_nodes[i], &nodes_count, &leaf_count);
            printf("The bitcoin %d has been used in %d transactions\n", bitcoin_id, nodes_count - leaf_count);
            current_node = all_nodes[i];
            while (current_node->from_child != NULL)
            {
                current_node = current_node->from_child;
            }
            printf("The bitcoin %d has remaind amount of %f\n", bitcoin_id, current_node->value);
            return SUCCESS;
        }
    }
    print_error(BITCOIN_NOT_FOUND_ERROR, "Bticoin status");
    return BITCOIN_NOT_FOUND_ERROR;
}

int trace_coin(char *line, BitcoinTreeNode **all_nodes, int counter)
{
    char *token;
    int i;
    token = strtok(line, " ");
    token = strtok(NULL, " ");
    int bitcoin_id = atoi(token);

    for (i = 0; i <= counter; i++)
    {
        if (all_nodes[i]->id == bitcoin_id)
        {
            if (all_nodes[i]->from_child == NULL)
            {
                printf("No transactions for this bitcoin\n");
            }

            in_order_visit_history(all_nodes[i]);
            return SUCCESS;
        }
    }
    print_error(BITCOIN_NOT_FOUND_ERROR, "Bticoin status");
    return BITCOIN_NOT_FOUND_ERROR;
}

int word_count(const char *line, int length)
{
    int count = 1;
    for (int i = 0; i < length; i++)
    {
        if (line[i] == ' ')
        {
            count++;
        }
    }
    return count;
}

void get_datetime_tokens(char *input_string, char *time1, char *time2, char *date1, char *date2)
{
    int time_count1 = 0;
    int time_count2 = 0;
    char *start = input_string;
    char *end = input_string;
    while (*input_string)
    {
        if (*input_string == '[')
            start = input_string;
        else if (*input_string == ']')
            end = input_string;
        if (start < end && *start)
        {
            *end = 0;
            if (strlen(start + 1) == 5)
            {
                if (time_count1 == 0)
                {
                    strcpy(time1, start + 1);
                    time_count1++;
                }
                else
                {
                    strcpy(time2, start + 1);
                }
            }
            else
            {
                if (time_count2 == 0)
                {
                    strcpy(date1, start + 1);
                    time_count2++;
                }
                else
                {
                    strcpy(date2, start + 1);
                }
            }
            start = input_string = end;
        }
        input_string++;
    }
    return;
}

void in_order_visit(BitcoinTreeNode *node, int *count, int *leaf_count)
{
    if (node != NULL)
    {
        in_order_visit(node->from_child, count, leaf_count);

        (*count)++;

        if (node->from_child == NULL && node->to_child == NULL)
        {
            (*leaf_count)++;
        }

        in_order_visit(node->to_child, count, leaf_count);
    }
}

void in_order_visit_history(BitcoinTreeNode *node)
{
    if (node != NULL)
    {
        in_order_visit_history(node->from_child);
        if (node->transaction_node != NULL)
        {
            printf("%s %s %s %f %s %s\n",
                   node->transaction_node->transaction->id,
                   node->transaction_node->transaction->sender_id,
                   node->transaction_node->transaction->receiver_id,
                   node->transaction_node->transaction->amount,
                   node->transaction_node->transaction->date,
                   node->transaction_node->transaction->time);
        }
        in_order_visit_history(node->to_child);
    }
}

int get_next_generator_value()
{
    last_id++;
    return last_id;
}