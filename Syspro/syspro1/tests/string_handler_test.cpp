#include "gtest/gtest.h"
#include "../io/io_manager.h"
#include "../error_handler/error_handler.h"
#include "../string/string_handler.h"

namespace
{

class StringHandlerTest : public testing::Test
{
  protected:
    StringHandlerTest()
    {
    }

    ~StringHandlerTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(StringHandlerTest, CreateStringTest)
{
    int result_code = 0;
    char *test_string = create_string(1, &result_code);
    test_string[0] = 'a';

    ASSERT_EQ(strlen(test_string), 1);
    ASSERT_EQ(test_string[1], '\0');

    destroy_string(test_string);
}

TEST_F(StringHandlerTest, CopyStringTest)
{
    int result_code = 0;
    char *test_string = create_string(1, &result_code);
    test_string[0] = 'a';

    char *copied_string = copy_string(test_string, &result_code);

    ASSERT_EQ(strlen(copied_string), 1);
    ASSERT_EQ(copied_string[1], '\0');
    ASSERT_EQ(copied_string[0], test_string[0]);

    destroy_string(test_string);
    destroy_string(copied_string);
}

TEST_F(StringHandlerTest, CopyConstStringTest)
{
    int result_code = 0;

    char *copied_string = copy_const_string("a", &result_code);

    ASSERT_EQ(strlen(copied_string), 1);
    ASSERT_EQ(copied_string[1], '\0');
    ASSERT_EQ(copied_string[0], 'a');

    destroy_string(copied_string);
}

TEST_F(StringHandlerTest, ConcatDateTime)
{
    int result_code;
    char *date = copy_const_string("11-11-1111", &result_code);
    char *time = copy_const_string("11:11", &result_code);

    char *result = concat_date_time(date, time, &result_code);
    ASSERT_STREQ(result, "11-11-1111-11:11");

    destroy_string(result);
    destroy_string(date);
    destroy_string(time);
}

TEST_F(StringHandlerTest, AddIdToTransaction)
{
    int result_code;
    char *input_string = copy_const_string("richard crystalsmith 20 13-02-2018 12:32", &result_code);

    char *result = add_id_to_transaction(input_string, 2, &result_code);

    ASSERT_STREQ(result, "2 richard crystalsmith 20 13-02-2018 12:32");

    destroy_string(input_string);
    destroy_string(result);
}

TEST_F(StringHandlerTest, AddDateTimeToTransaction)
{
    int result_code;
    char *input_string = copy_const_string("2 a b 3", &result_code);

    char *result = add_date_time_to_transaction(input_string, &result_code);

    ASSERT_EQ(strlen(result), 24);

    destroy_string(input_string);
    destroy_string(result);
}

TEST_F(StringHandlerTest, DestroyStringTest)
{
    int result_code = 0;
    char *test_string = create_string(1, &result_code);
    destroy_string(test_string);
}

}; // namespace