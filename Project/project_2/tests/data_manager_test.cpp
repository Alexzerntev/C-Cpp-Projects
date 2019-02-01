#include "gtest/gtest.h"
#include <unistd.h>
#include "../common/data_manipulation/data_manager.hpp"
#include "../common/data_manipulation/data_item.hpp"

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
  ASSERT_EQ(result->size(), 1);
  ASSERT_DOUBLE_EQ(result->at(0)->vector_data->at(0), -0.041663746140279171726);
  ASSERT_DOUBLE_EQ(result->at(0)->vector_data->at(result->at(0)->vector_data->size() - 1), 0.0037102372969338992537);

  DataItem<double> *temp;
  while (result->size() > 0)
  {
    temp = result->at(0);
    result->erase(result->begin());
    delete temp;
  }
}

} // namespace