#include "gtest/gtest.h"
#include <string>
#include "../common/argument_initialize.hpp"
#include "../common/enums.hpp"

namespace
{

// The fixture for testing class Foo.
class ArgumentInitializeTest : public testing::Test
{
  protected:
    ArgumentInitializeTest()
    {
    }

    ~ArgumentInitializeTest() override
    {
    }

    void SetUp() override
    {
        input_chars_[0] = new char[10];
        strcpy(input_chars_[0], "./cluster");

        input_chars_[1] = new char[3];
        strcpy(input_chars_[1], "-i");

        input_chars_[2] = new char[22];
        strcpy(input_chars_[2], "input_files/input.dat");

        input_chars_[3] = new char[3];
        strcpy(input_chars_[3], "-c");

        input_chars_[4] = new char[25];
        strcpy(input_chars_[4], "input_files/cluster.conf");

        input_chars_[5] = new char[3];
        strcpy(input_chars_[5], "-o");

        input_chars_[6] = new char[19];
        strcpy(input_chars_[6], "result/test_result");

        input_chars_[7] = new char[3];
        strcpy(input_chars_[7], "-d");

        input_chars_[8] = new char[7];
        strcpy(input_chars_[8], "cosine");

        argument_initialize_ = new ArgumentInitialize(9, input_chars_);
    }

    void TearDown() override
    {
        delete argument_initialize_;
        argument_initialize_ = NULL;

        for (int i = 0; i < 9; i++)
        {
            delete[] input_chars_[i];
        }
    }
    char *input_chars_[9];
    ArgumentInitialize *argument_initialize_;
};

TEST_F(ArgumentInitializeTest, Initialize)
{
    ASSERT_STREQ(argument_initialize_->get_config_file().c_str(), "input_files/cluster.conf");
    ASSERT_STREQ(argument_initialize_->get_input_file().c_str(), "input_files/input.dat");
    ASSERT_STREQ(argument_initialize_->get_output_file().c_str(), "result/test_result");
    ASSERT_EQ(argument_initialize_->get_metric(), COSINE);
}

} // namespace