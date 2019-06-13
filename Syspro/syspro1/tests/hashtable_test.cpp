#include "gtest/gtest.h"
#include "../error_handler/error_handler.h"
#include "../string/string_handler.h"
#include "../data_structures/hashtable/hashtable.h"

namespace
{

class HashtableTest : public testing::Test
{
  protected:
    HashtableTest()
    {
    }

    ~HashtableTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(HashtableTest, CreateHashtable)
{
    int result_code;
    Hashtable *hashtable = create_hashtable(3, 2, &result_code);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    ASSERT_EQ(hashtable->size, 3);
    ASSERT_EQ(hashtable->buckets[0]->filled, 0);
    ASSERT_EQ(hashtable->buckets[0]->size, 2);
    ASSERT_TRUE(hashtable->buckets[0]->users[0] == NULL);
    ASSERT_TRUE(hashtable->buckets[0]->users[1] == NULL);

    ASSERT_EQ(hashtable->buckets[1]->filled, 0);
    ASSERT_EQ(hashtable->buckets[1]->size, 2);
    ASSERT_TRUE(hashtable->buckets[1]->users[0] == NULL);
    ASSERT_TRUE(hashtable->buckets[1]->users[1] == NULL);

    ASSERT_EQ(hashtable->buckets[2]->filled, 0);
    ASSERT_EQ(hashtable->buckets[2]->size, 2);
    ASSERT_TRUE(hashtable->buckets[2]->users[0] == NULL);
    ASSERT_TRUE(hashtable->buckets[2]->users[1] == NULL);

    destroy_hashtable(hashtable, 1);
}

TEST_F(HashtableTest, CreateBucket)
{
    int result_code;
    Bucket *bucket = create_bucket(&result_code, 2);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    ASSERT_EQ(bucket->filled, 0);
    ASSERT_EQ(bucket->size, 2);
    ASSERT_TRUE(bucket->users[0] == NULL);
    ASSERT_TRUE(bucket->users[1] == NULL);
    ASSERT_TRUE(bucket->next_bucket == NULL);

    free(bucket->users);
    free(bucket);
}

TEST_F(HashtableTest, AddUserToBitcoinHashtable)
{
    int result_code, index, dump = 0;
    Hashtable *hashtable = create_hashtable(3, 2, &result_code);

    char *user_string = copy_const_string("id1 1 2 3", &result_code);
    User *user = create_bitcoin_user(user_string, 10, NULL, &dump);

    index = get_hash(user->id, 3);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    result_code = add_user_to_bitcoin_hashtable(hashtable, user);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    ASSERT_EQ(hashtable->buckets[index]->filled, 1);
    ASSERT_EQ(hashtable->buckets[index]->size, 2);
    ASSERT_STREQ(hashtable->buckets[index]->users[0]->id, "id1");
    ASSERT_EQ(hashtable->buckets[index]->users[0]->bitcoins->id, 3);
    ASSERT_EQ(hashtable->buckets[index]->users[0]->bitcoins->value, 10);
    ASSERT_EQ(hashtable->buckets[index]->users[0]->bitcoins->next->id, 2);
    ASSERT_EQ(hashtable->buckets[index]->users[0]->bitcoins->next->next->id, 1);
    ASSERT_TRUE(hashtable->buckets[index]->users[1] == NULL);

    destroy_string(user_string);
    destroy_hashtable(hashtable, 0);
}

TEST_F(HashtableTest, AddUserToBitcoinBucket)
{
    int result_code, dump = 0;

    char *user_string = copy_const_string("id1 1 2 3", &result_code);
    char *user_string1 = copy_const_string("id4 4 5 6", &result_code);
    char *user_string2 = copy_const_string("id7 7 8 9", &result_code);
    User *user = create_bitcoin_user(user_string, 10, NULL, &dump);
    User *user1 = create_bitcoin_user(user_string1, 10, NULL, &dump);
    User *user2 = create_bitcoin_user(user_string2, 10, NULL, &dump);

    Bucket *bucket = create_bucket(&result_code, 2);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    result_code = add_user_to_bitcoin_bucket(bucket, user);
    result_code = add_user_to_bitcoin_bucket(bucket, user1);
    result_code = add_user_to_bitcoin_bucket(bucket, user2);

    ASSERT_EQ(bucket->filled, 2);
    ASSERT_EQ(bucket->size, 2);
    ASSERT_EQ(bucket->next_bucket->size, 2);
    ASSERT_EQ(bucket->next_bucket->filled, 1);

    ASSERT_STREQ(bucket->users[0]->id, "id1");
    ASSERT_EQ(bucket->users[0]->bitcoins->id, 3);
    ASSERT_EQ(bucket->users[0]->bitcoins->value, 10);
    ASSERT_EQ(bucket->users[0]->bitcoins->next->id, 2);
    ASSERT_EQ(bucket->users[0]->bitcoins->next->next->id, 1);

    ASSERT_STREQ(bucket->users[1]->id, "id4");
    ASSERT_EQ(bucket->users[1]->bitcoins->id, 6);
    ASSERT_EQ(bucket->users[1]->bitcoins->value, 10);
    ASSERT_EQ(bucket->users[1]->bitcoins->next->id, 5);
    ASSERT_EQ(bucket->users[1]->bitcoins->next->next->id, 4);

    ASSERT_STREQ(bucket->next_bucket->users[0]->id, "id7");
    ASSERT_EQ(bucket->next_bucket->users[0]->bitcoins->id, 9);
    ASSERT_EQ(bucket->next_bucket->users[0]->bitcoins->value, 10);
    ASSERT_EQ(bucket->next_bucket->users[0]->bitcoins->next->id, 8);
    ASSERT_EQ(bucket->next_bucket->users[0]->bitcoins->next->next->id, 7);
    ASSERT_TRUE(bucket->next_bucket->users[1] == NULL);

    destroy_string(user_string);
    destroy_string(user_string1);
    destroy_string(user_string2);

    free(bucket->next_bucket->users[0]);
    free(bucket->next_bucket->users);
    free(bucket->next_bucket);

    free(bucket->users[0]);
    free(bucket->users[1]);
    free(bucket->users);
    free(bucket);
}

TEST_F(HashtableTest, AddToTransactionHashtable)
{
    int result_code;
    char *input_string = copy_const_string("23 id1 id4 20 13-02-2018 12:32", &result_code);
    char last_date[11] = "12-02-2018";
    char last_time[6] = "11:10";
    TransactionListNode *transaction = tokenize_and_create_transaction_list_node(input_string, last_date, last_time, &result_code);
    TransactionListNode *transaction1 = tokenize_and_create_transaction_list_node(input_string, last_date, last_time, &result_code);

    Hashtable *receiver_hashtable = create_hashtable(3, 2, &result_code);
    Hashtable *sender_hashtable = create_hashtable(3, 2, &result_code);

    result_code = add_to_transaction_hashtable(receiver_hashtable, sender_hashtable, transaction);
    result_code = add_to_transaction_hashtable(receiver_hashtable, sender_hashtable, transaction1);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    int index = get_hash(transaction->transaction->receiver_id, 3);

    ASSERT_EQ(receiver_hashtable->buckets[index]->filled, 1);
    ASSERT_EQ(receiver_hashtable->buckets[index]->size, 2);
    ASSERT_STREQ(receiver_hashtable->buckets[index]->users[0]->id, "id4");

    ASSERT_EQ(sender_hashtable->buckets[index]->filled, 1);
    ASSERT_EQ(sender_hashtable->buckets[index]->size, 2);
    ASSERT_STREQ(sender_hashtable->buckets[index]->users[0]->id, "id1");

    ASSERT_TRUE(sender_hashtable->buckets[index]->users[0]->transction_list->transaction == receiver_hashtable->buckets[index]->users[0]->transction_list->transaction);
    ASSERT_TRUE(sender_hashtable->buckets[index]->users[0]->transction_list->next->transaction == receiver_hashtable->buckets[index]->users[0]->transction_list->next->transaction);

    destroy_hashtable(receiver_hashtable, 0);
    destroy_hashtable(sender_hashtable, 1);
    destroy_string(input_string);

    // Add to transaction bucket is already tested by this method
}

TEST_F(HashtableTest, FindUser)
{
    int result_code, dump = 0;
    Hashtable *hashtable = create_hashtable(3, 2, &result_code);

    char *user_string = copy_const_string("id1 1 2 3", &result_code);
    char *user_string1 = copy_const_string("id4 4 5 6", &result_code);
    char *user_string2 = copy_const_string("id194 5 6", &result_code);
    User *user = create_bitcoin_user(user_string, 10, NULL, &dump);
    User *user1 = create_bitcoin_user(user_string1, 10, NULL, &dump);
    User *user2 = create_bitcoin_user(user_string2, 10, NULL, &dump);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    result_code = add_user_to_bitcoin_hashtable(hashtable, user);
    result_code = add_user_to_bitcoin_hashtable(hashtable, user1);

    ASSERT_STREQ(user->id, find_user(hashtable, user->id, &result_code)->id);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    ASSERT_TRUE(find_user(hashtable, user2->id, &result_code) == NULL);

    destroy_string(user_string);
    destroy_string(user_string1);
    destroy_string(user_string2);
    destroy_user(user2, 1);
    destroy_hashtable(hashtable, 0);
}

TEST_F(HashtableTest, ExecuteTransaction)
{
    int result_code, dump = 0;
    Hashtable *hashtable = create_hashtable(3, 2, &result_code);

    char *user_string = copy_const_string("id1 1 2 3", &result_code);
    char *user_string1 = copy_const_string("id4 4 5 6", &result_code);
    char *date = copy_const_string("11-11-1111", &result_code);
    char *time = copy_const_string("11:11", &result_code);
    User *user = create_bitcoin_user(user_string, 10, NULL, &dump);
    User *user1 = create_bitcoin_user(user_string1, 10, NULL, &dump);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    result_code = add_user_to_bitcoin_hashtable(hashtable, user);
    result_code = add_user_to_bitcoin_hashtable(hashtable, user1);

    TransactionListNode *transaction_list = create_transaction_node(1, user->id, user1->id, 17, date, time);
    result_code = execute_transaction(hashtable, transaction_list);

    ASSERT_EQ(user1->bitcoins->next->next->next->id, 3);
    // in case needed
    // ASSERT_EQ(user1->bitcoins->next->next->next->transaction_node->transaction->id, 1);
    // ASSERT_EQ(user1->bitcoins->next->next->next->transaction_node->transaction->amount, 17);
    // ASSERT_STREQ(user1->bitcoins->next->next->next->transaction_node->transaction->time, "11:11");
    // ASSERT_STREQ(user1->bitcoins->next->next->next->transaction_node->transaction->date, "11-11-1111");
    ASSERT_EQ(user1->bitcoins->next->next->next->value, 10);
    ASSERT_EQ(user1->bitcoins->next->next->next->next->value, 7);
    ASSERT_EQ(user1->bitcoins->next->next->next->next->id, 2);

    if (result_code != SUCCESS)
    {
        FAIL();
    }

    destroy_string(user_string);
    destroy_string(user_string1);
    destroy_string(date);
    destroy_string(time);
    destroy_hashtable(hashtable, 0);
}

TEST_F(HashtableTest, GetHash)
{
    int result_code;
    char *user_string = copy_const_string("id1", &result_code);
    int hash = get_hash(user_string, 3);
    ASSERT_EQ(hash, 2);
    destroy_string(user_string);
}

}; // namespace