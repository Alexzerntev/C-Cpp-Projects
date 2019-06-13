#include "gtest/gtest.h"
#include "../error_handler/error_handler.h"
#include "../data_structures/bitcoin_tree/bitcoin_tree.h"
#include "../string/string_handler.h"
#include "../model/user/user.h"

namespace
{

class UserTest : public testing::Test
{
protected:
  UserTest()
  {
  }

  ~UserTest() override
  {
  }

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(UserTest, CreateBitcoinUser)
{
  int result_code, dump = 0;
  char *user_string = copy_const_string("id1 1 2 3", &result_code);
  User *user = create_bitcoin_user(user_string, 10, NULL, &dump);

  ASSERT_STREQ(user->id, "id1");
  ASSERT_EQ(strlen(user->id), 3);
  ASSERT_EQ(user->bitcoins->value, 10);
  ASSERT_EQ(user->bitcoins->id, 3);

  ASSERT_EQ(user->bitcoins->next->value, 10);
  ASSERT_EQ(user->bitcoins->next->id, 2);

  ASSERT_EQ(user->bitcoins->next->next->value, 10);
  ASSERT_EQ(user->bitcoins->next->next->id, 1);

  destroy_user(user, 1);
  destroy_string(user_string);
}

TEST_F(UserTest, CreateTransactionUser)
{
  int result_code;
  char *user_id = copy_const_string("id1", &result_code);
  User *user = create_transaction_user(user_id);

  ASSERT_STREQ(user->id, "id1");
  ASSERT_TRUE(user->transction_list == NULL);
  ASSERT_TRUE(user->bitcoins == NULL);

  destroy_string(user_id);
  destroy_user(user, 1);
}

}; // namespace