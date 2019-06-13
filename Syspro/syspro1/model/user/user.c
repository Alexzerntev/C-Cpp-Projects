#include "./user.h"

User *create_bitcoin_user(char *user_entry, double bitcoin_value, BitcoinTreeNode **all_nodes, int *counter)
{
    int result_code, id;
    User *user = (User *)malloc(sizeof(User));

    if (user == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "User creation");
        return NULL;
    }

    user->bitcoins = NULL;
    user->id = NULL;
    user->transction_list = NULL;
    user->current_amount = 0;

    char *temp_string = copy_string(user_entry, &result_code);
    remove_line_break(temp_string);

    if (result_code != SUCCESS)
    {
        print_error(result_code, "User creation string copy");
        return NULL;
    }

    char *token;
    token = strtok(temp_string, " ");

    if (token == NULL)
    {
        print_error(STRING_PARSING_ERROR, "User Id");
        return NULL;
    }

    user->id = copy_string(token, &result_code);
    if (result_code != SUCCESS)
    {
        print_error(result_code, "User creation");
        return NULL;
    }
    token = strtok(NULL, " ");
    int flag = 0;
    while (token != NULL)
    {
        id = atoi(token);

        // checking for duplicate bitcoins
        for (int i = 0; i < (*counter); i++)
        {
            if (id == all_nodes[i]->id)
            {
                printf("Duplicate bitcoin id: %d\n", id);
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            flag = 0;
            continue;
        }

        if (user->bitcoins == NULL)
        {
            user->bitcoins = create_bitcoin_node(id, bitcoin_value, all_nodes, counter);
            user->bitcoins->is_root = 1;
            user->current_amount += bitcoin_value;
            token = strtok(NULL, " ");
            continue;
        }
        result_code = add_bitcoin_node(&user->bitcoins, id, bitcoin_value, all_nodes, counter);
        user->bitcoins->is_root = 1;
        user->current_amount += bitcoin_value;
        if (result_code != SUCCESS)
        {
            return NULL;
        }
        token = strtok(NULL, " ");
    }

    destroy_string(temp_string);
    return user;
}

User *create_transaction_user(char *user_id)
{
    int result_code;
    User *user = (User *)malloc(sizeof(User));

    if (user == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "User creation");
        return NULL;
    }

    user->bitcoins = NULL;
    user->transction_list = NULL;
    user->id = NULL;
    user->current_amount = 0;

    user->id = copy_string(user_id, &result_code);
    if (result_code != SUCCESS)
    {
        print_error(result_code, "Transaction user creation");
        return NULL;
    }

    return user;
}

void destroy_user(User *user, int will_destroy_transaction)
{
    if (user->bitcoins == NULL)
    {
        destroy_transaction_nodes(user->transction_list, will_destroy_transaction);
    }
    else
    {
        destroy_bitcoin_nodes(user->bitcoins);
    }

    destroy_string(user->id);
    free(user);
    return;
}