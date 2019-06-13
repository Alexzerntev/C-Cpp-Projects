#include "gtest/gtest.h"
#include "../error_handler/error_handler.h"
#include "../data_structures/transaction_list/transaction_list.h"
#include "../string/string_handler.h"

namespace
{

class TransactionsTest : public testing::Test
{
protected:
  TransactionsTest()
  {
  }

  ~TransactionsTest() override
  {
  }

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(TransactionsTest, CreateTransactionNode)
{
  int resulut_code;
  char *sender_id = copy_const_string("id1", &resulut_code);
  char *receiver_id = copy_const_string("id2", &resulut_code);
  char *date = copy_const_string("11-02-2019", &resulut_code);
  char *time = copy_const_string("10:00", &resulut_code);
  TransactionListNode *tln = create_transaction_node(0,
                                                     sender_id,
                                                     receiver_id,
                                                     10,
                                                     date,
                                                     time);

  ASSERT_STREQ(tln->transaction->id, "0");
  ASSERT_EQ(tln->transaction->amount, 10);
  ASSERT_STREQ(tln->transaction->date, "11-02-2019");
  ASSERT_STREQ(tln->transaction->time, "10:00");
  ASSERT_STREQ(tln->transaction->sender_id, "id1");
  ASSERT_STREQ(tln->transaction->receiver_id, "id2");

  destroy_transaction_nodes(tln, 1);

  destroy_string(sender_id);
  destroy_string(receiver_id);
  destroy_string(date);
  destroy_string(time);
}

TEST_F(TransactionsTest, AddTransactionNode)
{
  int resulut_code;
  char *sender_id = copy_const_string("id1", &resulut_code);
  char *receiver_id = copy_const_string("id2", &resulut_code);
  char *sender_id1 = copy_const_string("id3", &resulut_code);
  char *receiver_id1 = copy_const_string("id4", &resulut_code);
  char *date = copy_const_string("11-02-2019", &resulut_code);
  char *time = copy_const_string("10:00", &resulut_code);
  char *date1 = copy_const_string("13-03-2019", &resulut_code);
  char *time1 = copy_const_string("10:30", &resulut_code);
  TransactionListNode *tln = create_transaction_node(0,
                                                     sender_id,
                                                     receiver_id,
                                                     10,
                                                     date,
                                                     time);

  TransactionListNode *tln1 = create_transaction_node(1,
                                                      sender_id1,
                                                      receiver_id1,
                                                      10,
                                                      date1,
                                                      time1);
  resulut_code = add_transaction_node(&tln, tln1);

  ASSERT_STREQ(tln->next->transaction->id, "0");
  ASSERT_EQ(tln->next->transaction->amount, 10);
  ASSERT_STREQ(tln->next->transaction->date, "11-02-2019");
  ASSERT_STREQ(tln->next->transaction->time, "10:00");
  ASSERT_STREQ(tln->next->transaction->sender_id, "id1");
  ASSERT_STREQ(tln->next->transaction->receiver_id, "id2");

  ASSERT_STREQ(tln->transaction->id, "1");
  ASSERT_EQ(tln->transaction->amount, 10);
  ASSERT_STREQ(tln->transaction->date, "13-03-2019");
  ASSERT_STREQ(tln->transaction->time, "10:30");
  ASSERT_STREQ(tln->transaction->sender_id, "id3");
  ASSERT_STREQ(tln->transaction->receiver_id, "id4");

  destroy_transaction_nodes(tln, 1);

  destroy_string(sender_id);
  destroy_string(receiver_id);
  destroy_string(sender_id1);
  destroy_string(receiver_id1);
  destroy_string(date);
  destroy_string(time);
  destroy_string(date1);
  destroy_string(time1);
}

TEST_F(TransactionsTest, CreateTransaction)
{
  int resulut_code;
  char *sender_id = copy_const_string("id1", &resulut_code);
  char *receiver_id = copy_const_string("id2", &resulut_code);
  char *date = copy_const_string("11-02-2019", &resulut_code);
  char *time = copy_const_string("10:00", &resulut_code);

  Transaction *tl = create_transaction(0,
                                       sender_id,
                                       receiver_id,
                                       10,
                                       date,
                                       time);

  ASSERT_STREQ(tl->id, "0");
  ASSERT_EQ(tl->amount, 10);
  ASSERT_STREQ(tl->date, "11-02-2019");
  ASSERT_STREQ(tl->time, "10:00");
  ASSERT_STREQ(tl->sender_id, "id1");
  ASSERT_STREQ(tl->receiver_id, "id2");

  destroy_transaction(tl);

  destroy_string(sender_id);
  destroy_string(receiver_id);
  destroy_string(date);
  destroy_string(time);
}

TEST_F(TransactionsTest, TokenizeAndCreateTransactionListNode)
{
  int result_code;
  char *input_string = copy_const_string("23 richard crystalsmith 20 13-02-2018 12:32", &result_code);

  char last_date[11] = "11-11-1111";
  char last_time[6] = "11:11";
  TransactionListNode *tln = tokenize_and_create_transaction_list_node(input_string, last_date, last_time, &result_code);

  ASSERT_STREQ(tln->transaction->id, "23");
  ASSERT_EQ(tln->transaction->amount, 20);
  ASSERT_STREQ(tln->transaction->sender_id, "richard");
  ASSERT_STREQ(tln->transaction->receiver_id, "crystalsmith");
  ASSERT_STREQ(tln->transaction->date, "13-02-2018");
  ASSERT_STREQ(tln->transaction->time, "12:32");

  destroy_transaction_nodes(tln, 1);
  destroy_string(input_string);
}

TEST_F(TransactionsTest, CopyTransctionNode)
{
  int result_code;
  char *sender_id = copy_const_string("id1", &result_code);
  char *receiver_id = copy_const_string("id2", &result_code);
  char *date = copy_const_string("11-02-2019", &result_code);
  char *time = copy_const_string("10:00", &result_code);
  TransactionListNode *tln = create_transaction_node(0,
                                                     sender_id,
                                                     receiver_id,
                                                     10,
                                                     date,
                                                     time);
  TransactionListNode *tln1 = copy_transaction_node(tln, &result_code);

  ASSERT_STREQ(tln1->transaction->id, "0");
  ASSERT_EQ(tln1->transaction->amount, 10);
  ASSERT_STREQ(tln1->transaction->date, "11-02-2019");
  ASSERT_STREQ(tln1->transaction->time, "10:00");
  ASSERT_STREQ(tln1->transaction->sender_id, "id1");
  ASSERT_STREQ(tln1->transaction->receiver_id, "id2");

  destroy_transaction_nodes(tln, 1);

  destroy_string(sender_id);
  destroy_string(receiver_id);
  destroy_string(date);
  destroy_string(time);
}

}; // namespace