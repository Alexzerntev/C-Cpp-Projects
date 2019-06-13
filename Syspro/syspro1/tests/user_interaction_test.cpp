#include "gtest/gtest.h"
#include <unistd.h>
#include <string>
#include "../error_handler/error_handler.h"
#include "../data_structures/bitcoin_tree/bitcoin_tree.h"
#include "../string/string_handler.h"
#include "../model/user/user.h"
#include "../io/user_interaction.h"
#include "../io/io_manager.h"

using namespace std;

namespace
{

class UserInteractionTest : public testing::Test
{
  protected:
    int result_code = 0;

    UserInteractionTest()
    {
    }

    ~UserInteractionTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(UserInteractionTest, InOrderVisit)
{
    int dump = 0, count = 0, leaf_count = 0;
    BitcoinTreeNode *node = create_bitcoin_node(0, 10, NULL, &dump);
    node->from_child = create_bitcoin_node(1, 10, NULL, &dump);
    node->to_child = create_bitcoin_node(2, 10, NULL, &dump);

    in_order_visit(node, &count, &leaf_count);

    ASSERT_EQ(count, 3);
    ASSERT_EQ(leaf_count, 2);

    free(node->to_child);
    free(node->from_child);
    free(node);
}

TEST_F(UserInteractionTest, GetDateTimeTokens)
{
    char *input_string = copy_const_string("[11-11-1111][11:10][11-11-1112][11:20]", &result_code);
    char time1[6], time2[6], date1[11], date2[11];
    time1[5] = '\0';
    time2[5] = '\0';
    date1[10] = '\0';
    date2[10] = '\0';
    get_datetime_tokens(input_string, time1, time2, date1, date2);

    ASSERT_STREQ(time1, "11:10");
    ASSERT_STREQ(time2, "11:20");

    ASSERT_STREQ(date1, "11-11-1111");
    ASSERT_STREQ(date2, "11-11-1112");

    char *input_string1 = copy_const_string("[12:10][12:20]", &result_code);

    get_datetime_tokens(input_string1, time1, time2, date1, date2);

    ASSERT_STREQ(time1, "12:10");
    ASSERT_STREQ(time2, "12:20");

    destroy_string(input_string);
}

TEST_F(UserInteractionTest, WordCount)
{
    int wc = word_count("a b c d", 7);
    ASSERT_EQ(wc, 4);
}

}; // namespace