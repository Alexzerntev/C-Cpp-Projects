#include "./initialize_cube.hpp"

InitializeCube::InitializeCube(int argc, char **argv)
    : InitializeBase(argc, argv), k_(3), M_(10), probes_(2)
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
        else if (temp_string == "-M")
        {
            temp_string = ++i < argc ? argv[i] : "";
            try
            {
                M_ = stoi(temp_string);
            }
            catch (const std::invalid_argument &e)
            {
                cout << "Invalid hash tables count" << endl;
                is_valid_ = false;
                return;
            }
        }
        else if (temp_string == "-probes")
        {
            temp_string = ++i < argc ? argv[i] : "";
            try
            {
                probes_ = stoi(temp_string);
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

void InitializeCube::manual_init(bool &is_first_time)
{
    InitializeBase::manual_init();

    for (int i = 5; i < 8; i++)
    {
        InitializeBase::user_interact((AttributeType)i);
    }
}

void InitializeCube::user_interact(AttributeType type)
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
            case C_LS_COUNT:
                k_ = stoi(result);
                return;
            case M_COUNT:
                M_ = stoi(result);
                return;
            case PROBES:
                probes_ = stoi(result);
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

int InitializeCube::get_k()
{
    return k_;
}

unsigned int InitializeCube::get_M()
{
    return M_;
}

int InitializeCube::get_probes()
{
    return probes_;
}