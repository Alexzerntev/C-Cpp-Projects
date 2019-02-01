#include "gtest/gtest.h"
#include <string>
#include <typeinfo>
#include <map>
#include "../common/enums.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../recomendation/recomendation_helpers.hpp"

namespace
{

class RecomendationHelpersTest : public testing::Test
{
  protected:
    RecomendationHelpersTest()
    {
    }

    ~RecomendationHelpersTest() override
    {
    }

    void SetUp() override
    {
        data = new vector<DataItem<double> *>();

        DataItem<double> *data1 = new DataItem<double>();
        data1->id = 15;
        data1->vector_data = new vector<double>();
        data1->vector_data->push_back(1);
        data1->vector_data->push_back(2);
        data1->is_known = new vector<bool>();
        data1->is_known->push_back(true);
        data1->is_known->push_back(true);
        data1->lsh_distance = 1;

        DataItem<double> *data2 = new DataItem<double>();
        data2->id = 10;
        data2->vector_data = new vector<double>();
        data2->is_known = new vector<bool>();
        data2->is_known->push_back(true);
        data2->is_known->push_back(true);
        data2->lsh_distance = 2;

        data2->vector_data->push_back(3);
        data2->vector_data->push_back(4);

        data->push_back(data1);
        data->push_back(data2);
    }

    void TearDown() override
    {
        DataItem<double> *temp;
        while (data->size() > 0)
        {
            temp = data->at(0);
            data->erase(data->begin());
            delete temp;
        }
        delete data;
    }

    vector<DataItem<double> *> *data;
};

TEST_F(RecomendationHelpersTest, GetUserIndex)
{
    ASSERT_EQ(get_user_index(data, 15), 0);
    ASSERT_EQ(get_user_index(data, 10), 1);
    ASSERT_EQ(data->size(), 2);
}

TEST_F(RecomendationHelpersTest, Normalize)
{
    normalize_data(data);
    ASSERT_EQ(data->at(0)->vector_data->at(0), -0.5);
    ASSERT_EQ(data->at(0)->vector_data->at(1), 0.5);
}

TEST_F(RecomendationHelpersTest, GetCoinIndex)
{
    vector<string> *coins = new vector<string>();
    coins->push_back("coin1");
    coins->push_back("coin2");
    coins->push_back("coin3");
    coins->push_back("coin4");
    ASSERT_EQ(get_coin_index(coins, "coin1"), 0);
    ASSERT_EQ(get_coin_index(coins, "coin2"), 1);
    ASSERT_EQ(get_coin_index(coins, "coin3"), 2);
    ASSERT_EQ(get_coin_index(coins, "coin4"), 3);
    delete coins;
}

TEST_F(RecomendationHelpersTest, ExecuteRecomendation)
{
    DataItem<double> *user = new DataItem<double>();
    DataItem<double> *result;

    user->id = 13;
    user->is_known = new vector<bool>();
    user->is_known->push_back(false);
    user->is_known->push_back(true);
    user->vector_data = new vector<double>();
    user->vector_data->push_back(0);
    user->vector_data->push_back(0);
    user->average_score = 2;
    result = execute_recomendation(user, data, false);

    ASSERT_NEAR(result->vector_data->at(0), 2.333, 3);

    delete user;
    delete result;
}

TEST_F(RecomendationHelpersTest, ClusterToUser)
{
    // FAIL();
}

} // namespace