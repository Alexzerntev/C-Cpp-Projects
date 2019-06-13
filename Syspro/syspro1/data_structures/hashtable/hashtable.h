#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../transaction_list/transaction_list.h"
#include "../hashtable/hashtable.h"
#include "../../error_handler/error_handler.h"
#include "../../model/user/user.h"

#define USER_HASHTABLE 0
#define TRANSACTIONS_HASHTABLE 1

typedef struct Bucket
{
    int size;   // in entries
    int filled; // in entries
    struct User **users;
    struct Bucket *next_bucket;
} Bucket;

typedef struct Hashtable
{
    int size;
    struct Bucket **buckets;

} Hashtable;

Hashtable *create_hashtable(int buckets_count,
                            int entries_per_bucket,
                            int *result_code);

int get_hash(char *id, int size);

Bucket *create_bucket(int *result_code, int entries_per_bucket);

int get_bucket_count(int users_count,
                     int bucket_size,
                     int *entries_per_bucket);

int get_entries_per_bucket(int bucket_size);

int add_user_to_bitcoin_hashtable(Hashtable *hashtable, User *user);

int add_user_to_bitcoin_bucket(Bucket *bucket, User *user);

User *find_user(Hashtable *hashtable, char *user_id, int *result_code);

int add_to_transaction_bucket(Bucket *receiver_bucket,
                              Bucket *sender_bucket,
                              TransactionListNode *transaction_node);

int add_to_transaction_hashtable(Hashtable *receiver_hashtable,
                                 Hashtable *sender_hashtable,
                                 TransactionListNode *transaction_node);

int execute_transaction(Hashtable *bitcoin_hashtable, TransactionListNode *transaction_node);

void destroy_hashtable(Hashtable *hashtable, int will_destroy_transaction);

#endif