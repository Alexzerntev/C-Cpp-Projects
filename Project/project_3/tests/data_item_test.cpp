#include "gtest/gtest.h"
#include <string>
#include <typeinfo>
#include <map>
#include "../common/enums.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../recomendation/recomendation_helpers.hpp"

namespace
{

class DataItemTest : public testing::Test
{
  protected:
    DataItemTest()
    {
    }

    ~DataItemTest() override
    {
    }

    void SetUp() override
    {
        data = new DataItem<double>();
        data->id = 15;
        data->vector_data = new vector<double>();
        data->vector_data->push_back(1);
        data->vector_data->push_back(2);
        data->vector_data->push_back(3);
        data->vector_data->push_back(4);
        data->vector_data->push_back(5);
        data->is_known = new vector<bool>();
        data->is_known->push_back(false);
        data->is_known->push_back(false);
        data->is_known->push_back(false);
        data->is_known->push_back(false);
        data->is_known->push_back(true);

        data1 = new DataItem<double>();
        data1->id = 15;
        data1->vector_data = new vector<double>();
        data1->vector_data->push_back(1);
        data1->vector_data->push_back(2);
        data1->is_known = new vector<bool>();
        data1->is_known->push_back(true);
        data1->is_known->push_back(false);

        coins = new vector<string>();
        coins->push_back("coin0");
        coins->push_back("coin1");
        coins->push_back("coin2");
        coins->push_back("coin3");
        coins->push_back("coin4");
    }

    void TearDown() override
    {
        delete data;
        delete data1;
        delete coins;
    }

    DataItem<double> *data;
    DataItem<double> *data1;
    vector<string> *coins;
};

TEST_F(DataItemTest, NormalizeTest)
{
    data1->normalize();
    ASSERT_NEAR(data1->vector_data->at(0), -0.5, 2);
    ASSERT_NEAR(data1->average_score, 1.5, 2);
}

TEST_F(DataItemTest, GetRecomended)
{
    vector<string> result;
    result = data->get_recomended(coins, 3);
    int count = 0;
    for (unsigned int i = 0; i < result.size(); i++)
    {
        if (result.at(i) == "coin3" || result.at(i) == "coin2" || result.at(i) == "coin1")
        {
            count++;
        }
    }
    ASSERT_EQ(count, 3);
}

} // namespace