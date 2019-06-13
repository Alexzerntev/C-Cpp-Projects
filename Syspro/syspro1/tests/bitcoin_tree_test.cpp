#include "gtest/gtest.h"
#include "../error_handler/error_handler.h"
#include "../data_structures/bitcoin_tree/bitcoin_tree.h"
#include "../string/string_handler.h"

namespace
{

class BitcoinTreeTest : public testing::Test
{
  protected:
    BitcoinTreeTest()
    {
    }

    ~BitcoinTreeTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(BitcoinTreeTest, CreateNodeTest)
{
    int dump = 0;
    BitcoinTreeNode *node = create_bitcoin_node(0, 20, NULL, &dump);
    ASSERT_EQ(node->id, 0);
    ASSERT_TRUE(node->next == NULL);
    ASSERT_TRUE(node->to_child == NULL);
    ASSERT_TRUE(node->from_child == NULL);
    destroy_bitcoin_nodes(node);
}

TEST_F(BitcoinTreeTest, AddNodeTest)
{
    int dump = 0;
    BitcoinTreeNode *node = create_bitcoin_node(0, 10, NULL, &dump);
    int result = add_bitcoin_node(&node, 1, 20, NULL, &dump);
    if (result != SUCCESS)
    {
        FAIL();
    }
    result = add_bitcoin_node(&node, 2, 30, NULL, &dump);
    if (result != SUCCESS)
    {
        FAIL();
    }
    ASSERT_EQ(node->value, 30);
    ASSERT_EQ(node->id, 2);
    ASSERT_EQ(node->next->value, 20);
    ASSERT_EQ(node->next->id, 1);
    ASSERT_EQ(node->next->next->value, 10);
    ASSERT_EQ(node->next->next->id, 0);

    ASSERT_TRUE(node->next->next->next == NULL);
    destroy_bitcoin_nodes(node);
}

TEST_F(BitcoinTreeTest, AddBitcoinTransactionNodeTest)
{
    int dump = 0;
    BitcoinTreeNode *node = create_bitcoin_node(1, 10, NULL, &dump);
    BitcoinTreeNode *node1 = create_bitcoin_node(2, 20, NULL, &dump);
    BitcoinTreeNode *node2 = create_bitcoin_node(3, 30, NULL, &dump);
    add_bitcoin_transaction_node(node, node1);
    add_bitcoin_transaction_node(node, node2);

    ASSERT_EQ(node->value, 10);
    ASSERT_EQ(node->id, 1);
    ASSERT_EQ(node->next->value, 20);
    ASSERT_EQ(node->next->id, 2);
    ASSERT_EQ(node->next->next->value, 30);
    ASSERT_EQ(node->next->next->id, 3);

    ASSERT_TRUE(node->next->next->next == NULL);
    destroy_bitcoin_nodes(node);
}

}; // namespace