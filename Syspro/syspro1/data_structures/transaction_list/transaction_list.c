#include "./transaction_list.h"

TransactionListNode *create_transaction_node(int id,
                                             char *sender_id,
                                             char *receiver_id,
                                             double amount,
                                             char *date,
                                             char *time)
{
    TransactionListNode *node = (TransactionListNode *)malloc(sizeof(TransactionListNode));
    if (node == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Transaction node creation");
        return NULL;
    }
    node->previous = NULL;
    node->next = NULL;
    node->transaction = create_transaction(id, sender_id, receiver_id, amount, date, time);
    return node;
}

Transaction *create_transaction(int id,
                                char *sender_id,
                                char *receiver_id,
                                double amount,
                                char *date,
                                char *time)
{
    int result_code;
    Transaction *transaction = (Transaction *)malloc(sizeof(Transaction));

    if (transaction == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Transaction creation");
        return NULL;
    }

    transaction->id = NULL;
    transaction->amount = 0;
    transaction->receiver_id = NULL;
    transaction->sender_id = NULL;

    char buffer[12];
    sprintf(buffer, "%d", id);

    transaction->id = create_string(strlen(buffer), &result_code);

    if (result_code != SUCCESS)
    {
        print_error(result_code, "Transaction Creation");
        return NULL;
    }

    strcpy(transaction->id, buffer);

    transaction->amount = amount;

    transaction->receiver_id = copy_string(receiver_id, &result_code);
    if (result_code != SUCCESS)
    {
        print_error(result_code, "Transaction creation");
        return NULL;
    }

    transaction->sender_id = copy_string(sender_id, &result_code);
    if (result_code != SUCCESS)
    {
        print_error(result_code, "Transaction creation");
        return NULL;
    }

    memcpy(transaction->date, date, 10);
    transaction->date[10] = '\0';
    memcpy(transaction->time, time, 5);
    transaction->time[5] = '\0';

    return transaction;
}

int add_transaction_node(TransactionListNode **start_node, TransactionListNode *to_be_added)
{
    if (to_be_added == NULL)
    {
        return MEMORY_ALLOCATION_ERROR;
    }

    to_be_added->next = *start_node;
    *start_node = to_be_added;
    to_be_added->next->previous = to_be_added;
    return SUCCESS;
}

TransactionListNode *tokenize_and_create_transaction_list_node(char *transaction_data, char *last_date, char *last_time, int *result_code)
{
    TransactionListNode *transaction_node;
    int transaction_id;
    char *sender_id, *receiver_id, *date, *time;
    double amount;

    char *temp_string = copy_string(transaction_data, result_code);
    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Adding transaction to bucket");
        return NULL;
    }

    char *token;
    token = strtok(temp_string, " ");

    if (token == NULL)
    {
        print_error(STRING_PARSING_ERROR, "User Id");
        return NULL;
    }

    transaction_id = atoi(token);
    token = strtok(NULL, " ");

    if (token == NULL)
    {
        print_error(STRING_PARSING_ERROR, "Sender Id");
        return NULL;
    }

    sender_id = copy_string(token, result_code);
    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Tokenizing transaction");
        return NULL;
    }

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        print_error(STRING_PARSING_ERROR, "Reciever Id");
        return NULL;
    }

    receiver_id = copy_string(token, result_code);
    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Tokenizing transaction");
        return NULL;
    }

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        print_error(STRING_PARSING_ERROR, "Amount");
        return NULL;
    }
    amount = atof(token);

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        print_error(STRING_PARSING_ERROR, "Date");
        return NULL;
    }

    date = copy_string(token, result_code);
    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Tokenizing transaction");
        return NULL;
    }

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        print_error(STRING_PARSING_ERROR, "Time");
        return NULL;
    }

    time = copy_string(token, result_code);
    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Tokenizing transaction");
        return NULL;
    }

    if (compare_date_time(date, time, last_date, last_time) > 0)
    {
        print_error(0, "Transaction is before last transaction");
        *result_code = WRONG_INPUT;
        return NULL;
    }

    transaction_node = create_transaction_node(transaction_id, sender_id, receiver_id, amount, date, time);

    strcpy(last_date, date);
    strcpy(last_time, time);

    destroy_string(receiver_id);
    destroy_string(sender_id);
    destroy_string(date);
    destroy_string(time);
    destroy_string(temp_string);

    return transaction_node;
}

TransactionListNode *copy_transaction_node(TransactionListNode *node, int *result_code)
{
    TransactionListNode *new_node = (TransactionListNode *)malloc(sizeof(TransactionListNode));
    new_node->next = NULL;
    new_node->previous = NULL;
    if (new_node == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    new_node->transaction = node->transaction;
    *result_code = SUCCESS;
    return new_node;
}

void destroy_transaction(Transaction *transaction)
{
    destroy_string(transaction->receiver_id);
    destroy_string(transaction->sender_id);
    destroy_string(transaction->id);
    free(transaction);
}

void destroy_transaction_nodes(TransactionListNode *transaction_list_node, int will_destroy_transaction)
{
    TransactionListNode *temp;
    while (transaction_list_node != NULL)
    {
        temp = transaction_list_node;
        transaction_list_node = transaction_list_node->next;
        if (will_destroy_transaction == 1)
        {
            destroy_transaction(temp->transaction);
            temp->transaction = NULL;
        }

        free(temp);
    }
}
