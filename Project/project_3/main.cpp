#include <iostream>
#include "./execution.hpp"
#include "./validate_execution.hpp"

int main(int argc, char *argv[])
{
    string temp;
    bool will_validate = false;
    for (int i = 0; i < argc; i++)
    {
        temp = argv[i];
        if (temp == "-validate")
        {
            will_validate = true;
        }
    }

    if (will_validate == false)
    {
        Execution *execution = new Execution();

        execution->users_lsh(argc, argv);
        // execution->users_cluster(argc, argv);
        // execution->virtual_users_lsh(argc, argv);
        // execution->virtual_users_cluster(argc, argv);

        delete execution;
    }
    else
    {
        ValidateExecution *validate_execution = new ValidateExecution();

        // validate_execution->users_lsh(argc, argv);
        // validate_execution->users_cluster(argc, argv);
        validate_execution->virtual_users_lsh(argc, argv);
        // validate_execution->virtual_users_cluster(argc, argv);

        delete validate_execution;
    }

    return 0;
}