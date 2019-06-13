#include "./server_settings.h"

ServerSettings *create_server_settings(int *result_code)
{
    ServerSettings *settings = (ServerSettings *)malloc(sizeof(ServerSettings));
    if (settings == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Could not create settings");
    }

    settings->port_num = -1;

    return settings;
}

int validate_and_store_server_settings(char **console_input, int size, ServerSettings *settings)
{
    if (size != 3)
    {
        return WRONG_INITIALIZATION_ERROR;
    }

    int i;
    for (i = 1; i < size; i++)
    {
        if (strcmp(console_input[i], "-p") == 0)
        {
            i++;
            settings->port_num = atoi(console_input[i]);

            if (settings->port_num == 0 && strlen(console_input[i]) != 1)
            {
                print_error(INT_CONVERTION_ERROR, "Wrong id");
                return INT_CONVERTION_ERROR;
            }

            continue;
        }
        else
        {
            return WRONG_INITIALIZATION_ERROR;
        }
    }

    if (settings->port_num == -1)
    {
        return WRONG_INITIALIZATION_ERROR;
    }

    return SUCCESS;
}

void destroy_server_settings(ServerSettings *settings)
{
    free(settings);
}