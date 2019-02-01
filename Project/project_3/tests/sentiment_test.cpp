#include "gtest/gtest.h"
#include <string>
#include <typeinfo>
#include <map>
#include "../common/argument_initialize.hpp"
#include "../common/enums.hpp"
#include "../recomendation/sentiment.hpp"
#include "../common/data_manipulation/user_data_item.hpp"

namespace
{

class SentimentTest : public testing::Test
{
  protected:
    SentimentTest()
    {
    }

    ~SentimentTest() override
    {
    }

    void SetUp() override
    {
        sentiment = new Sentiment<double>();
        dictionary = new map<string, double>();
        user_data = new vector<UserDataItem *>();
        data = new vector<DataItem<double> *>();

        coin_dictionary = new map<string, string>();
        coins = new vector<string>();

        UserDataItem *udt1 = new UserDataItem();
        udt1->tweet_data = new vector<string>();
        udt1->tweet_data->push_back("test1");
        udt1->tweet_data->push_back("test2");
        udt1->tweet_data->push_back("btc");
        udt1->tweet_id = 1;
        udt1->user_id = 1;

        UserDataItem *udt2 = new UserDataItem();
        udt2->tweet_data = new vector<string>();
        udt2->tweet_data->push_back("test3");
        udt2->tweet_data->push_back("test4");
        udt2->tweet_data->push_back("eth");
        udt2->tweet_id = 2;
        udt2->user_id = 2;

        user_data->push_back(udt1);
        user_data->push_back(udt2);

        DataItem<double> *dt1 = new DataItem<double>();
        dt1->vector_data = new vector<double>(2, 0);
        dt1->id = 1;
        dt1->is_known = new vector<bool>(2, false);

        DataItem<double> *dt2 = new DataItem<double>();
        dt2->vector_data = new vector<double>(2, 0);
        dt2->id = 2;
        dt2->is_known = new vector<bool>(2, false);

        data->push_back(dt1);
        data->push_back(dt2);

        dictionary->insert(pair<string, double>("test1", 1));
        dictionary->insert(pair<string, double>("test2", -1.5));
        dictionary->insert(pair<string, double>("test3", 2.2));
        dictionary->insert(pair<string, double>("test4", -3.1));

        coin_dictionary->insert(pair<string, string>("btc", "btc"));
        coin_dictionary->insert(pair<string, string>("$btc", "btc"));

        coin_dictionary->insert(pair<string, string>("eth", "eth"));
        coin_dictionary->insert(pair<string, string>("$eth", "eth"));

        coins->push_back("btc");
        coins->push_back("eth");
    }

    void TearDown() override
    {
        UserDataItem *temp1;
        while (user_data->size() > 0)
        {
            temp1 = user_data->at(0);
            user_data->erase(user_data->begin());
            delete temp1;
        }

        DataItem<double> *temp2;
        while (data->size() > 0)
        {
            temp2 = data->at(0);
            data->erase(data->begin());
            delete temp2;
        }

        delete sentiment;
        delete dictionary;
        delete user_data;
        delete coin_dictionary;
        delete coins;
        delete data;
    }

    Sentiment<double> *sentiment;
    map<string, double> *dictionary;
    map<string, string> *coin_dictionary;
    vector<UserDataItem *> *user_data;
    vector<DataItem<double> *> *data;
    vector<string> *coins;
};

TEST_F(SentimentTest, AssignSentiment)
{
    sentiment->assign_sentiment(user_data, dictionary);
    ASSERT_NEAR(user_data->at(0)->sentiment, -0.12803687993, 4);
    ASSERT_NEAR(user_data->at(1)->sentiment, -0.22634795467, 4);
}

TEST_F(SentimentTest, FillDataItems)
{
    ASSERT_EQ(data->at(0)->vector_data->size(), 2);
    ASSERT_EQ(data->at(1)->vector_data->size(), 2);
    sentiment->fill_data_items(data, coins, user_data, coin_dictionary);
    ASSERT_NEAR(data->at(0)->vector_data->at(0), -0.12803687993, 4);
    ASSERT_EQ(data->at(0)->vector_data->at(1), 0);

    ASSERT_EQ(data->at(1)->vector_data->at(0), 0);
    ASSERT_NEAR(data->at(1)->vector_data->at(1), -0.22634795467, 4);
}

TEST_F(SentimentTest, RemoveZeroes)
{
    // FAIL();
}

} // namespace