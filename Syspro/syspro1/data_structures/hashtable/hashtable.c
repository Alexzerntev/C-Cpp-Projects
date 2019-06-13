#include "./hashtable.h"

// creates a hashtable without data in buckets
Hashtable *create_hashtable(int buckets_count,
                            int entries_per_bucket,
                            int *result_code)
{
    int i;
    Hashtable *hashtable = (Hashtable *)malloc(sizeof(Hashtable));
    if (hashtable == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Hashtable creation");
        return NULL;
    }

    hashtable->buckets = NULL;
    hashtable->size = 0;
    hashtable->buckets = (Bucket **)malloc(sizeof(Bucket *) * buckets_count);

    if (hashtable->buckets == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Bucket array creation");
        return NULL;
    }

    for (i = 0; i < buckets_count; i++)
    {
        hashtable->buckets[i] = NULL;
    }

    hashtable->size = buckets_count;

    for (i = 0; i < buckets_count; i++)
    {
        hashtable->buckets[i] = create_bucket(result_code, entries_per_bucket);
        if (*result_code != SUCCESS)
        {
            return NULL;
        }
    }

    *result_code = SUCCESS;
    return hashtable;
}

Bucket *create_bucket(int *result_code, int entries_per_bucket)
{
    int i;
    Bucket *bucket = (Bucket *)malloc(sizeof(Bucket));

    if (bucket == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Bucket creation");
        return NULL;
    }

    bucket->next_bucket = NULL;
    bucket->users = (User **)malloc(sizeof(User *) * entries_per_bucket);
    for (i = 0; i < entries_per_bucket; i++)
    {
        bucket->users[i] = NULL;
    }

    bucket->size = entries_per_bucket;
    bucket->filled = 0;
    *result_code = SUCCESS;
    return bucket;
}

int add_user_to_bitcoin_hashtable(Hashtable *hashtable, User *user)
{
    int index, result_code;
    index = get_hash(user->id, hashtable->size);

    result_code = add_user_to_bitcoin_bucket(hashtable->buckets[index], user);

    if (result_code != SUCCESS)
    {
        print_error(result_code, "Adding user to hashtable");
        return result_code;
    }

    return SUCCESS;
}

int add_user_to_bitcoin_bucket(Bucket *bucket, User *user)
{
    int i, result_code;
    Bucket *current_bucket = bucket;
    while (current_bucket->size == current_bucket->filled)
    {
        for (i = 0; i < current_bucket->size; i++)
        {
            if (strcmp(current_bucket->users[i]->id, user->id) == 0)
            {
                return DUPLICATE_USER_ERROR;
            }
        }
        current_bucket = current_bucket->next_bucket;
    }

    if (current_bucket->size == (current_bucket->filled + 1))
    {
        current_bucket->next_bucket = create_bucket(&result_code, bucket->size);
        if (result_code != SUCCESS)
        {
            return result_code;
        }
    }
    current_bucket->users[current_bucket->filled] = user;
    current_bucket->filled++;
    return SUCCESS;
}

User *find_user(Hashtable *hashtable, char *user_id, int *result_code)
{
    int index = get_hash(user_id, hashtable->size);
    int i;
    Bucket *current_bucket = hashtable->buckets[index];
    while (current_bucket != NULL)
    {
        for (i = 0; i < current_bucket->filled; i++)
        {
            if (strcmp(current_bucket->users[i]->id, user_id) == 0)
            {
                *result_code = SUCCESS;
                return current_bucket->users[i];
            }
        }
        current_bucket = current_bucket->next_bucket;
    }
    *result_code = USER_NOT_FOUND_ERROR;
    return NULL;
}

int execute_transaction(Hashtable *bitcoin_hashtable, TransactionListNode *transaction_node)
{
    int dump = 0;
    int result_code;
    User *user = find_user(bitcoin_hashtable, transaction_node->transaction->sender_id, &result_code);
    User *receiver = find_user(bitcoin_hashtable, transaction_node->transaction->receiver_id, &result_code);
    if (user == NULL)
    {
        printf("%s\n", transaction_node->transaction->sender_id);
        print_error(USER_NOT_FOUND_ERROR, "execute_transaction");
        return USER_NOT_FOUND_ERROR;
    }
    if (receiver == NULL)
    {
        printf("%s\n", transaction_node->transaction->receiver_id);
        print_error(USER_NOT_FOUND_ERROR, "execute_transaction");
        return USER_NOT_FOUND_ERROR;
    }

    if (user->current_amount < transaction_node->transaction->amount)
    {
        printf("From %s to %s amount of %f when remained %f\n", transaction_node->transaction->sender_id, transaction_node->transaction->receiver_id, transaction_node->transaction->amount, user->current_amount);
        print_error(NOT_ENOUGH_MONEY_ERROR, "execute_transaction");
        return NOT_ENOUGH_MONEY_ERROR;
    }

    BitcoinTreeNode *current_node = user->bitcoins;
    BitcoinTreeNode *root = user->bitcoins;

    double current_sum = transaction_node->transaction->amount, temp_value;

    while (current_sum != 0)
    {
        // go to the most left leaf
        while (current_node->from_child != NULL)
        {
            current_node = current_node->from_child;
        }

        //to initialize the first node

        if (current_node->value == 0)
        {
            root = root->next;
            current_node = root;
            continue;
        }

        temp_value = current_node->value - current_sum;
        current_node->transaction_node = transaction_node;

        if (temp_value < 0)
        {
            current_node->from_child = create_bitcoin_node(root->id, 0, NULL, &dump);
            current_node->to_child = create_bitcoin_node(root->id, current_node->value, NULL, &dump);
            current_sum -= current_node->value;
            if (receiver->bitcoins == NULL)
            {
                receiver->bitcoins = current_node->to_child;
            }
            else
            {
                add_bitcoin_transaction_node(receiver->bitcoins, current_node->to_child);
            }
        }
        else
        {
            current_node->from_child = create_bitcoin_node(root->id, temp_value, NULL, &dump);
            current_node->to_child = create_bitcoin_node(root->id, current_node->value - temp_value, NULL, &dump);
            current_sum -= transaction_node->transaction->amount;
            if (receiver->bitcoins == NULL)
            {
                receiver->bitcoins = current_node->to_child;
            }
            else
            {
                add_bitcoin_transaction_node(receiver->bitcoins, current_node->to_child);
            }
            break;
        }

        root = root->next;
        current_node = root;
    }

    receiver->current_amount += transaction_node->transaction->amount;
    user->current_amount -= transaction_node->transaction->amount;

    return SUCCESS;
}

int add_to_transaction_hashtable(Hashtable *receiver_hashtable,
                                 Hashtable *sender_hashtable,
                                 TransactionListNode *transaction_node)
{
    int result_code;
    int receiver_index = get_hash(transaction_node->transaction->receiver_id, receiver_hashtable->size);
    int sender_index = get_hash(transaction_node->transaction->sender_id, sender_hashtable->size);
    result_code = add_to_transaction_bucket(receiver_hashtable->buckets[receiver_index],
                                            sender_hashtable->buckets[sender_index],
                                            transaction_node);
    return result_code;
}

int add_to_transaction_bucket(Bucket *receiver_bucket,
                              Bucket *sender_bucket,
                              TransactionListNode *transaction_node)
{
    int i, result_code, exists = 0;

    // ============================= Sender part ==============================

    Bucket *current_bucket = sender_bucket;
    Bucket *previous_bucket;
    while (current_bucket != NULL)
    {
        for (i = 0; i < current_bucket->filled; i++)
        {
            if (strcmp(current_bucket->users[i]->id, transaction_node->transaction->sender_id) == 0)
            {
                add_transaction_node(&current_bucket->users[i]->transction_list, transaction_node);
                exists = 1;
                break;
            }
        }
        previous_bucket = current_bucket;
        current_bucket = current_bucket->next_bucket;
    }

    if (exists == 0)
    {
        if (previous_bucket->size == (previous_bucket->filled + 1))
        {
            previous_bucket->next_bucket = create_bucket(&result_code, sender_bucket->size);
            if (result_code != SUCCESS)
            {
                return result_code;
            }
        }

        User *user = create_transaction_user(transaction_node->transaction->sender_id);
        user->transction_list = transaction_node;
        previous_bucket->users[previous_bucket->filled] = user;
        previous_bucket->filled++;
    }

    // ============================ Receiver part =============================
    TransactionListNode *receiver_node = copy_transaction_node(transaction_node, &result_code);
    if (result_code != SUCCESS)
    {
        print_error(result_code, "Adding to transaction bucket");
        return result_code;
    }

    current_bucket = receiver_bucket;
    while (current_bucket != NULL)
    {
        for (i = 0; i < current_bucket->filled; i++)
        {
            if (strcmp(current_bucket->users[i]->id, receiver_node->transaction->receiver_id) == 0)
            {
                add_transaction_node(&current_bucket->users[i]->transction_list, receiver_node);
                return SUCCESS;
            }
        }
        previous_bucket = current_bucket;
        current_bucket = current_bucket->next_bucket;
    }

    if (previous_bucket->size == (previous_bucket->filled + 1))
    {
        previous_bucket->next_bucket = create_bucket(&result_code, receiver_bucket->size);
        if (result_code != SUCCESS)
        {
            return result_code;
        }
    }

    User *user = create_transaction_user(receiver_node->transaction->receiver_id);
    user->transction_list = receiver_node;
    previous_bucket->users[previous_bucket->filled] = user;
    previous_bucket->filled++;

    return SUCCESS;
}

void destroy_hashtable(Hashtable *hashtable, int will_destroy_transaction)
{
    int i, j;
    Bucket *current_bucket, *temp_bucket;
    for (i = 0; i < hashtable->size; i++)
    {
        current_bucket = hashtable->buckets[i];
        while (current_bucket != NULL)
        {
            for (j = 0; j < current_bucket->filled; j++)
            {
                destroy_user(current_bucket->users[j], will_destroy_transaction);
            }
            temp_bucket = current_bucket;
            current_bucket = current_bucket->next_bucket;
            free(temp_bucket->users);
            free(temp_bucket);
        }
    }
    free(hashtable->buckets);
    free(hashtable);
    return;
}

int get_hash(char *id, int size)
{
    unsigned long int sum = 0;
    int count = strlen(id);
    int i = 0;

    for (i = 0; i < count; i++)
    {
        sum += (id[i]);
    }

    return sum % size;
}

int get_bucket_count(int users_count,
                     int bucket_size,
                     int *entries_per_bucket)
{
    *entries_per_bucket = bucket_size / sizeof(User *) + 1;
    return (users_count / (0.7 * (*entries_per_bucket))) + 1;
}

int get_entries_per_bucket(int bucket_size)
{
    return (bucket_size / sizeof(User *)) + 1;
}