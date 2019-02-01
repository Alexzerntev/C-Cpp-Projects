#include "./argument_initialize.hpp"

ArgumentInitialize::ArgumentInitialize(int argc, char *argv[])
    : input_file_(""), config_file_(""), output_file_(""), is_complete_(false)
{
    try
    {
        for (int i = 0; i < argc; i++)
        {
            string temp_string = argv[i];
            if (temp_string == "-i")
            {
                temp_string = ++i < argc ? argv[i] : "";
                input_file_ = temp_string;
            }
            else if (temp_string == "-c")
            {
                temp_string = ++i < argc ? argv[i] : "";
                config_file_ = temp_string;
            }
            else if (temp_string == "-o")
            {
                temp_string = ++i < argc ? argv[i] : "";
                output_file_ = temp_string;
            }
            else if (temp_string == "-d")
            {
                temp_string = ++i < argc ? argv[i] : "";
                if (temp_string == "cosine")
                {
                    metric_ = COSINE;
                }
                else if (temp_string == "euclidean")
                {
                    metric_ = EUCLIDEAN;
                }
                else
                {
                    throw "wrong metric";
                }
            }
            else if (temp_string == "-complete")
            {
                is_complete_ = true;
            }
        }
        if (input_file_ == "" || config_file_ == "" || output_file_ == "")
        {
            throw "wrong arguments format";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

string ArgumentInitialize::get_input_file()
{
    return input_file_;
}

string ArgumentInitialize::get_config_file()
{
    return config_file_;
}

string ArgumentInitialize::get_output_file()
{
    return output_file_;
}

bool ArgumentInitialize::get_is_complete()
{
    return is_complete_;
}

MetricType ArgumentInitialize::get_metric()
{
    return metric_;
}