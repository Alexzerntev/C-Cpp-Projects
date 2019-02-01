#include "gtest/gtest.h"
#include <unistd.h>
#include <map>
#include "../common/data_manipulation/data_manager.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../common/data_manipulation/user_data_item.hpp"

namespace
{

// The fixture for testing class Foo.
class DataManagerTest : public testing::Test
{
protected:
  DataManagerTest()
  {
  }

  ~DataManagerTest() override
  {
  }

  void SetUp() override
  {
    data_manager_ = new DataManager<double>();
  }

  void TearDown() override
  {
    delete data_manager_;
    data_manager_ = NULL;
  }

  DataManager<double> *data_manager_;
};

TEST_F(DataManagerTest, SplitAndConvert)
{
  DataItem<double> *result = data_manager_->split_and_convert("1,0.3,0.0,2.0008,3.334");
  ASSERT_EQ(result->id, 1);
  ASSERT_EQ(result->vector_data->size(), 4);
  ASSERT_DOUBLE_EQ(result->vector_data->at(0), 0.3);
  ASSERT_DOUBLE_EQ(result->vector_data->at(result->vector_data->size() - 1), 3.334);
  delete result;
  ASSERT_ANY_THROW(data_manager_->split_and_convert("asdf asdf, afds"));
}

TEST_F(DataManagerTest, SplitAndConvertUserData)
{
  UserDataItem *result = data_manager_->split_and_convert_user_data("9\t15\tasdf\tasd");
  ASSERT_EQ(result->tweet_id, 15);
  ASSERT_EQ(result->user_id, 9);
  ASSERT_EQ(result->tweet_data->size(), 2);
  ASSERT_STREQ(result->tweet_data->at(0).c_str(), "asdf");
  ASSERT_STREQ(result->tweet_data->at(1).c_str(), "asd");
  delete result;
  ASSERT_ANY_THROW(data_manager_->split_and_convert_user_data("asdf asdf, afds"));
}

TEST_F(DataManagerTest, ReadSettings)
{
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);

  if (path.find("tests") != std::string::npos)
  {
    data_manager_->read_config("mockups/cluster.conf");
  }
  else
  {
    data_manager_->read_config("tests/mockups/cluster.conf");
  }

  ASSERT_EQ(data_manager_->get_number_of_clusters(), 3);
  ASSERT_EQ(data_manager_->get_number_of_hash_functions(), 3);
  ASSERT_EQ(data_manager_->get_number_of_hash_tables(), 3);
  ASSERT_EQ(data_manager_->get_probes(), 2);
  ASSERT_EQ(data_manager_->get_M(), 1000);
  ASSERT_ANY_THROW(data_manager_->read_config("asnfasfd"));
}

TEST_F(DataManagerTest, ReadData)
{
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);
  vector<DataItem<double> *> *result;

  if (path.find("tests") != std::string::npos)
  {
    result = data_manager_->read_data("mockups/input.dat");
  }
  else
  {
    result = data_manager_->read_data("tests/mockups/input.dat");
  }

  ASSERT_ANY_THROW(data_manager_->read_data("asdfysfd"));
  ASSERT_EQ(result->at(0)->id, 1);
  ASSERT_EQ(result->at(0)->vector_data->size(), 203);
  ASSERT_EQ(result->size(), 4);
  ASSERT_DOUBLE_EQ(result->at(0)->vector_data->at(0), -0.041663746140279171726);
  ASSERT_DOUBLE_EQ(result->at(0)->vector_data->at(result->at(0)->vector_data->size() - 1), 0.0037102372969338992537);

  DataItem<double> *temp;
  while (result->size() > 0)
  {
    temp = result->at(0);
    result->erase(result->begin());
    delete temp;
  }
  delete result;
}

TEST_F(DataManagerTest, ReadUserData)
{
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);
  vector<UserDataItem *> *result;

  if (path.find("tests") != std::string::npos)
  {
    result = data_manager_->read_user_data("mockups/input2.dat");
  }
  else
  {
    result = data_manager_->read_user_data("tests/mockups/input2.dat");
  }

  ASSERT_ANY_THROW(data_manager_->read_data("asdfysfd"));
  ASSERT_EQ(result->at(0)->user_id, 8);
  ASSERT_EQ(result->at(result->size() - 1)->user_id, 11);

  ASSERT_EQ(result->at(0)->tweet_id, 2631);
  ASSERT_EQ(result->at(result->size() - 1)->tweet_id, 4411);

  ASSERT_STREQ(result->at(0)->tweet_data->at(0).c_str(), "buy");
  ASSERT_STREQ(result->at(result->size() - 1)->tweet_data->at(0).c_str(), "...");

  ASSERT_STREQ(result->at(0)->tweet_data->at(result->at(0)->tweet_data->size() - 1).c_str(), "50");
  ASSERT_STREQ(result->at(result->size() - 1)->tweet_data->at(result->at(result->size() - 1)->tweet_data->size() - 1).c_str(), "...");

  UserDataItem *temp;
  while (result->size() > 0)
  {
    temp = result->at(0);
    result->erase(result->begin());
    delete temp;
  }
  delete result;
}

TEST_F(DataManagerTest, CreateSentimentDictionary)
{
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);
  map<string, double> *result;

  if (path.find("tests") != std::string::npos)
  {
    result = data_manager_->create_sentiment_dictionary("mockups/vader_lexicon.csv");
  }
  else
  {
    result = data_manager_->create_sentiment_dictionary("tests/mockups/vader_lexicon.csv");
  }

  ASSERT_ANY_THROW(data_manager_->create_sentiment_dictionary("asdfysfd"));
  ASSERT_EQ(result->at("$:"), -1.5);
  ASSERT_EQ(result->at("&-:"), -0.4);
  ASSERT_EQ(result->size(), 4);
  delete result;
}

TEST_F(DataManagerTest, CreateCoinDictionary)
{
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);
  map<string, string> *result;

  if (path.find("tests") != std::string::npos)
  {
    result = data_manager_->create_coin_dictionary("mockups/coins_queries.csv");
  }
  else
  {
    result = data_manager_->create_coin_dictionary("tests/mockups/coins_queries.csv");
  }

  ASSERT_ANY_THROW(data_manager_->create_coin_dictionary("asdfysfd"));
  ASSERT_STREQ(result->at("btc").c_str(), "btc");
  ASSERT_STREQ(result->at("@bitcoin").c_str(), "btc");
  ASSERT_STREQ(result->at("$ethereum").c_str(), "eth");
  ASSERT_EQ(result->size(), 16);
  delete result;
}

TEST_F(DataManagerTest, GetDistinctCoinNames)
{
  char cwd[200];
  getcwd(cwd, sizeof(cwd));
  string path(cwd);
  vector<string> *result;

  if (path.find("tests") != std::string::npos)
  {
    result = data_manager_->get_distinct_coin_names("mockups/coins_queries.csv");
  }
  else
  {
    result = data_manager_->get_distinct_coin_names("tests/mockups/coins_queries.csv");
  }

  ASSERT_ANY_THROW(data_manager_->create_coin_dictionary("asdfysfd"));
  ASSERT_STREQ(result->at(0).c_str(), "btc");
  ASSERT_STREQ(result->at(1).c_str(), "eth");
  ASSERT_EQ(result->size(), 2);
  delete result;
}

TEST_F(DataManagerTest, CreateUserProfiles)
{
  vector<UserDataItem *> *data = new vector<UserDataItem *>();

  UserDataItem *udt1 = new UserDataItem();
  udt1->tweet_data = new vector<string>();
  udt1->tweet_data->push_back("test1");
  udt1->tweet_data->push_back("test2");
  udt1->tweet_id = 1;
  udt1->user_id = 1;

  UserDataItem *udt2 = new UserDataItem();
  udt2->tweet_data = new vector<string>();
  udt2->tweet_data->push_back("test3");
  udt2->tweet_data->push_back("test4");
  udt2->tweet_id = 2;
  udt2->user_id = 2;

  UserDataItem *udt3 = new UserDataItem();
  udt3->tweet_data = new vector<string>();
  udt3->tweet_data->push_back("test9");
  udt3->tweet_data->push_back("test8");
  udt3->tweet_id = 3;
  udt3->user_id = 1;

  data->push_back(udt1);
  data->push_back(udt2);
  data->push_back(udt3);

  vector<string> *coins = new vector<string>();
  coins->push_back("coin1");
  coins->push_back("coin2");
  coins->push_back("coin3");
  coins->push_back("coin4");

  vector<DataItem<double> *> *result;
  result = data_manager_->create_user_profiles(data, coins);

  ASSERT_EQ(result->size(), 2);

  ASSERT_EQ(result->at(0)->id, 1);
  ASSERT_EQ(result->at(0)->vector_data->size(), 4);
  ASSERT_EQ(result->at(0)->vector_data->at(0), 0);
  ASSERT_EQ(result->at(0)->vector_data->at(result->at(0)->vector_data->size() - 1), 0);

  ASSERT_EQ(result->at(1)->id, 2);
  ASSERT_EQ(result->at(1)->vector_data->size(), 4);
  ASSERT_EQ(result->at(1)->vector_data->at(1), 0);
  ASSERT_EQ(result->at(1)->vector_data->at(result->at(1)->vector_data->size() - 1), 0);

  DataItem<double> *temp2;
  while (result->size() > 0)
  {
    temp2 = result->at(0);
    result->erase(result->begin());
    delete temp2;
  }

  UserDataItem *temp1;
  while (data->size() > 0)
  {
    temp1 = data->at(0);
    data->erase(data->begin());
    delete temp1;
  }
  delete data;
  delete result;
}

} // namespace