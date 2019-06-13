#include "gtest/gtest.h"
#include "../string/string_handler.h"
#include "../date_time/date_time.h"

namespace
{

class DateTimeTest : public testing::Test
{
protected:
  DateTimeTest()
  {
  }

  ~DateTimeTest() override
  {
  }

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(DateTimeTest, IsBetweenDateTime)
{
  int result_code;
  char *date1 = copy_const_string("22-06-1991", &result_code);
  char *date2 = copy_const_string("23-06-1991", &result_code);
  char *date3 = copy_const_string("24-06-1991", &result_code);
  char *time1 = copy_const_string("10:10", &result_code);
  char *time2 = copy_const_string("10:11", &result_code);
  char *time3 = copy_const_string("11:12", &result_code);

  ASSERT_EQ(is_between_date_time(date1, time1, date2, time2, date3, time3), 1);
  destroy_string(date1);
  destroy_string(date2);
  destroy_string(date3);
  destroy_string(time1);
  destroy_string(time2);
  destroy_string(time3);
}

TEST_F(DateTimeTest, CompareDateTime)
{
  int result_code;
  char *date1 = copy_const_string("22-06-1991", &result_code);
  char *date2 = copy_const_string("23-06-1991", &result_code);
  char *time1 = copy_const_string("10:10", &result_code);
  char *time2 = copy_const_string("10:11", &result_code);

  ASSERT_EQ(compare_date_time(date1, time1, date2, time2), 1);
  destroy_string(date1);
  destroy_string(date2);
  destroy_string(time1);
  destroy_string(time2);
}

TEST_F(DateTimeTest, GetNowDateTime)
{
  int result_code;
  char *date1 = get_now_date(&result_code);
  char *time1 = get_now_time(&result_code);

  ASSERT_EQ(strlen(date1), 10);
  ASSERT_EQ(strlen(time1), 5);

  destroy_string(date1);
  destroy_string(time1);
}

}; // namespace