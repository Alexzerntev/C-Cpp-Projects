#include "./initialize_base.hpp"

InitializeBase::InitializeBase(int argc, char *argv[])
    : input_file_(""), query_file_(""), output_file_("output"), is_valid_(true)
{
    for (int i = 0; i < argc; i++)
    {
        string temp_string = argv[i];
        if (temp_string == "-d")
        {
            temp_string = ++i < argc ? argv[i] : "";
            input_file_ = temp_string;
            if (input_file_ == "")
            {
                is_valid_ = false;
                return;
            }
        }
        else if (temp_string == "-q")
        {
            temp_string = ++i < argc ? argv[i] : "";
            query_file_ = temp_string;
            if (input_file_ == "")
            {
                is_valid_ = false;
                return;
            }
        }
        else if (temp_string == "-o")
        {
            temp_string = ++i < argc ? argv[i] : "";
            output_file_ = temp_string;
            if (input_file_ == "")
            {
                is_valid_ = false;
                return;
            }
        }
    }
}

bool InitializeBase::should_repeat()
{
    string answer;
    cout << "Would you like to repeat the proccess? y[es]/n[o]" << endl;
    cin >> answer;

    if (answer.at(0) == 'y' || answer.at(0) == 'Y')
    {
        is_valid_ = false;
        return true;
    }
    is_valid_ = true;
    return false;
}

void InitializeBase::user_interact(AttributeType type)
{
    bool success = false;
    while (!success)
    {
        cout << get_atr_message(type) << endl;
        try
        {
            string result = "";
            cin >> result;
            switch (type)
            {
            case INPUT:
                input_file_ = result;
                return;
            case OUTPUT:
                output_file_ = result;
                return;
            case QUERY:
                query_file_ = result;
                return;
            default:
                is_valid_ = false;
                return;
            }
        }
        catch (const std::exception &e)
        {
            cout << "Invalid Input, try again" << endl;
        }
    }
}

void InitializeBase::manual_init()
{
    for (int i = 0; i < 3; i++)
    {
        user_interact((AttributeType)i);
    }
}

string InitializeBase::get_input_file()
{
    return input_file_;
}

string InitializeBase::get_query_file()
{
    return query_file_;
}

string InitializeBase::get_output_file()
{
    return output_file_;
}

bool InitializeBase::is_valid()
{
    return is_valid_;
}
