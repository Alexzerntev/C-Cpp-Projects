#include "gtest/gtest.h"
#include <unistd.h>
#include "../io/io_manager.h"
#include "../model/settings/settings.h"
#include "../error_handler/error_handler.h"
#include "../string/string_handler.h"
#include <string>

using namespace std;

namespace
{

class IoManagerTest : public testing::Test
{
protected:
  IoManagerTest()
  {
  }

  ~IoManagerTest() override
  {
  }

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(IoManagerTest, ReadAndValidateInputTest)
{
  Settings settings;
  int i = 0;
  int result_code;
  char *test_string1[13]; // ./bitcoin -a bitCoinBalancesFile –t transactionsFile -v bitCoinValue –h1 senderHashtableNumOfEntries –h2 receiverHashtableNumOfEntries –b bucketSize
  test_string1[0] = copy_const_string("./bitcoin", &result_code);
  test_string1[1] = copy_const_string("-a", &result_code);
  test_string1[2] = copy_const_string("bitCoinBalancesFile", &result_code);
  test_string1[3] = copy_const_string("-t", &result_code);
  test_string1[4] = copy_const_string("transactionsFile", &result_code);
  test_string1[5] = copy_const_string("-v", &result_code);
  test_string1[6] = copy_const_string("1", &result_code);
  test_string1[7] = copy_const_string("-h1", &result_code);
  test_string1[8] = copy_const_string("2", &result_code);
  test_string1[9] = copy_const_string("-h2", &result_code);
  test_string1[10] = copy_const_string("3", &result_code);
  test_string1[11] = copy_const_string("-b", &result_code);
  test_string1[12] = copy_const_string("4", &result_code);

  char *test_string2[13]; // ./bitcoin -a bitCoinBalancesFile –t transactionsFile -v bitCoinValue –h1 senderHashtableNumOfEntries –h2 receiverHashtableNumOfEntries –b bucketSize
  test_string2[0] = copy_const_string("./bitcoin", &result_code);
  test_string2[3] = copy_const_string("-a", &result_code);
  test_string2[4] = copy_const_string("bitCoinBalancesFile", &result_code);
  test_string2[1] = copy_const_string("-t", &result_code);
  test_string2[2] = copy_const_string("transactionsFile", &result_code);
  test_string2[5] = copy_const_string("-v", &result_code);
  test_string2[6] = copy_const_string("1", &result_code);
  test_string2[7] = copy_const_string("-h1", &result_code);
  test_string2[8] = copy_const_string("2", &result_code);
  test_string2[9] = copy_const_string("-h2", &result_code);
  test_string2[10] = copy_const_string("3", &result_code);
  test_string2[11] = copy_const_string("-b", &result_code);
  test_string2[12] = copy_const_string("4", &result_code);

  ASSERT_EQ(validate_and_store(test_string1, 13, &settings), SUCCESS);
  ASSERT_EQ(settings.bitcoin_value, 1);
  ASSERT_EQ(settings.bucket_size, 4);
  ASSERT_EQ(settings.sender_hashtable_entries, 2);
  ASSERT_EQ(settings.reciever_hashtable_entries, 3);
  ASSERT_STREQ(settings.bitcoin_balances_file, "bitCoinBalancesFile");
  ASSERT_STREQ(settings.transactions_file, "transactionsFile");

  destroy_settings(&settings, 1);
  for (i = 0; i < 13; i++)
  {
    destroy_string(test_string1[i]);
  }

  ASSERT_EQ(validate_and_store(test_string2, 13, &settings), SUCCESS);
  ASSERT_EQ(settings.bitcoin_value, 1);
  ASSERT_EQ(settings.bucket_size, 4);
  ASSERT_EQ(settings.sender_hashtable_entries, 2);
  ASSERT_EQ(settings.reciever_hashtable_entries, 3);
  ASSERT_STREQ(settings.transactions_file, "transactionsFile");
  ASSERT_STREQ(settings.bitcoin_balances_file, "bitCoinBalancesFile");

  destroy_settings(&settings, 1);
  for (int i = 0; i < 13; i++)
  {
    destroy_string(test_string2[i]);
  }

  ASSERT_EQ(validate_and_store(NULL, 12, &settings), WRONG_INITIALIZATION_ERROR);
}

TEST_F(IoManagerTest, ReadBitcoinBalancesFile)
{
  int result_code, index, entries_per_bucket, dump = 0;;
  char *file_name;
  char *id = copy_const_string("id1", &result_code);
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);

  if (path.find("tests") != std::string::npos)
  {
    file_name = copy_const_string("mockups/bitcoin_balances.txt", &result_code);
  }
  else
  {
    file_name = copy_const_string("tests/mockups/bitcoin_balances.txt", &result_code);
  }

  Settings settings;
  settings.bucket_size = 100;
  settings.bitcoin_value = 50;

  Hashtable *hashtable = read_bitcoin_balances_file(file_name, &settings, &result_code, NULL, &dump);

  int buckets_count = get_bucket_count(2, 100, &entries_per_bucket);
  index = get_hash(id, buckets_count);

  ASSERT_EQ(hashtable->buckets[index]->size, entries_per_bucket);
  ASSERT_EQ(hashtable->size, buckets_count);
  ASSERT_STREQ(hashtable->buckets[index]->users[0]->id, "id1");
  ASSERT_EQ(hashtable->buckets[index]->users[0]->bitcoins->id, 3);
  ASSERT_EQ(hashtable->buckets[index]->users[0]->bitcoins->value, 50);
  ASSERT_EQ(hashtable->buckets[index]->users[0]->bitcoins->next->id, 2);
  ASSERT_EQ(hashtable->buckets[index]->users[0]->bitcoins->next->next->id, 1);

  ASSERT_STREQ(hashtable->buckets[index]->users[1]->id, "id4");
  ASSERT_EQ(hashtable->buckets[index]->users[1]->bitcoins->id, 6);
  ASSERT_EQ(hashtable->buckets[index]->users[1]->bitcoins->value, 50);
  ASSERT_EQ(hashtable->buckets[index]->users[1]->bitcoins->next->id, 5);
  ASSERT_EQ(hashtable->buckets[index]->users[1]->bitcoins->next->next->id, 4);

  ASSERT_STREQ(hashtable->buckets[index]->users[2]->id, "id7");
  ASSERT_EQ(hashtable->buckets[index]->users[2]->bitcoins->id, 9);
  ASSERT_EQ(hashtable->buckets[index]->users[2]->bitcoins->value, 50);
  ASSERT_EQ(hashtable->buckets[index]->users[2]->bitcoins->next->id, 8);
  ASSERT_EQ(hashtable->buckets[index]->users[2]->bitcoins->next->next->id, 7);

  destroy_string(file_name);
  destroy_string(id);
  destroy_hashtable(hashtable, 1);
}

TEST_F(IoManagerTest, ReadTransactionsFile)
{
  int result_code, index, dump = 0;
  char *file_name, *file_name1;
  char *id = copy_const_string("id1", &result_code);
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);

  if (path.find("tests") != std::string::npos)
  {
    file_name = copy_const_string("mockups/transactions.txt", &result_code);
  }
  else
  {
    file_name = copy_const_string("tests/mockups/transactions.txt", &result_code);
  }

  Settings settings;
  settings.bucket_size = 100;
  settings.bitcoin_value = 10;
  settings.reciever_hashtable_entries = 3;
  settings.sender_hashtable_entries = 3;

  Hashtable *receiver_hashtable;
  Hashtable *sender_hashtable;

  if (path.find("tests") != std::string::npos)
  {
    file_name1 = copy_const_string("mockups/bitcoin_balances.txt", &result_code);
  }
  else
  {
    file_name1 = copy_const_string("tests/mockups/bitcoin_balances.txt", &result_code);
  }

  Settings settings1;
  settings1.bucket_size = 100;
  settings1.bitcoin_value = 50;

  Hashtable *bitcoin_hashtable = read_bitcoin_balances_file(file_name1, &settings1, &result_code, NULL, &dump);
  char last_date[11] = "11-11-1111";
  char last_time[6] = "11:11";
  int last_id = 0;
  result_code = read_transactions_file(file_name, &settings, &receiver_hashtable, &sender_hashtable, bitcoin_hashtable, last_date, last_time, &last_id);
  index = get_hash(id, receiver_hashtable->size);

  ASSERT_EQ(receiver_hashtable->buckets[index]->filled, 1);
  ASSERT_EQ(receiver_hashtable->buckets[index]->size, 13);
  ASSERT_STREQ(receiver_hashtable->buckets[index]->users[0]->id, "id4");

  ASSERT_EQ(sender_hashtable->buckets[index]->filled, 1);
  ASSERT_EQ(sender_hashtable->buckets[index]->size, 13);
  ASSERT_STREQ(sender_hashtable->buckets[index]->users[0]->id, "id1");

  ASSERT_TRUE(sender_hashtable->buckets[index]->users[0]->transction_list->transaction == receiver_hashtable->buckets[index]->users[0]->transction_list->transaction);
  ASSERT_TRUE(sender_hashtable->buckets[index]->users[0]->transction_list->next->transaction == receiver_hashtable->buckets[index]->users[0]->transction_list->next->transaction);

  destroy_string(file_name);
  destroy_string(file_name1);
  destroy_string(id);
  destroy_hashtable(receiver_hashtable, 0);
  destroy_hashtable(sender_hashtable, 1);
}

TEST_F(IoManagerTest, GetAllBitcoins)
{
  int result_code;
  char *file_name;
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);

  if (path.find("tests") != std::string::npos)
  {
    file_name = copy_const_string("mockups/bitcoin_balances.txt", &result_code);
  }
  else
  {
    file_name = copy_const_string("tests/mockups/bitcoin_balances.txt", &result_code);
  }
  BitcoinTreeNode **all_bitcoins = get_all_of_bitcoins(file_name, &result_code);

  free(all_bitcoins);
}

}; // namespace