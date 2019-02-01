#include "./initialize_lsh.hpp"

InitializeLsh::InitializeLsh(int argc, char **argv)
    : InitializeBase(argc, argv), k_(4), L_(5)
{
    for (int i = 0; i < argc; i++)
    {
        string temp_string = argv[i];
        if (temp_string == "-k")
        {
            temp_string = ++i < argc ? argv[i] : "";
            try
            {
                k_ = stoi(temp_string);
            }
            catch (const std::invalid_argument &e)
            {
                cout << "Invalid locality sensitive count" << endl;
                is_valid_ = false;
                return;
            }
        }
        else if (temp_string == "-L")
        {
            temp_string = ++i < argc ? argv[i] : "";
            try
            {
                L_ = stoi(temp_string);
            }
            catch (const std::invalid_argument &e)
            {
                cout << "Invalid hash tables count" << endl;
                is_valid_ = false;
                return;
            }
        }
    }

    if (input_file_ == "" || query_file_ == "")
    {
        is_valid_ = false;
    }
}

void InitializeLsh::manual_init(bool &is_first_time)
{
    InitializeBase::manual_init();

    for (int i = 3; i < 5; i++)
    {
        InitializeBase::user_interact((AttributeType)i);
    }
}

void InitializeLsh::user_interact(AttributeType type)
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
            case LS_COUNT:
                k_ = stoi(result);
                return;
            case HASH_TABLES:
                L_ = stoi(result);
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

int InitializeLsh::get_k()
{
    return k_;
}

int InitializeLsh::get_L()
{
    return L_;
}