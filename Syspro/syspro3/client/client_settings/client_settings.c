#include "./client_settings.h"

ClientSettings *create_client_settings(int *result_code)
{
    ClientSettings *settings = (ClientSettings *)malloc(sizeof(ClientSettings));
    if (settings == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Could not create settings");
    }

    settings->buffer_size = -1;
    settings->dir_name = NULL;
    settings->port_num = -1;
    settings->server_ip = NULL;
    settings->server_port = -1;
    settings->worker_threads = -1;

    return settings;
}

int validate_and_store_client_settings(char **console_input, int size, ClientSettings *settings)
{
    int result_code = SUCCESS;
    if (size != 13)
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
                print_error(INT_CONVERTION_ERROR, "Port Num");
                return INT_CONVERTION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-d") == 0)
        {
            i++;
            settings->dir_name = copy_string(console_input[i], &result_code);

            if (result_code != SUCCESS)
            {
                print_error(result_code, "Dir Name");
                return result_code;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-w") == 0)
        {
            i++;
            settings->worker_threads = atoi(console_input[i]);

            if (settings->worker_threads == 0 && strlen(console_input[i]) != 1)
            {
                print_error(INT_CONVERTION_ERROR, "Worker Threads");
                return INT_CONVERTION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-sp") == 0)
        {
            i++;
            settings->server_port = atoi(console_input[i]);

            if (settings->server_port == 0 && strlen(console_input[i]) != 1)
            {
                print_error(INT_CONVERTION_ERROR, "Server Port");
                return INT_CONVERTION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-b") == 0)
        {
            i++;
            settings->buffer_size = atoi(console_input[i]);

            if (settings->buffer_size == 0 && strlen(console_input[i]) != 1)
            {
                print_error(INT_CONVERTION_ERROR, "Buffer size");
                return INT_CONVERTION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-sip") == 0)
        {
            i++;
            settings->server_ip = copy_string(console_input[i], &result_code);

            if (result_code != SUCCESS)
            {
                print_error(result_code, "Server Ip");
                return result_code;
            }

            continue;
        }
        else
        {
            return WRONG_INITIALIZATION_ERROR;
        }
    }

    if (settings->buffer_size == -1 ||
        settings->dir_name == NULL ||
        settings->port_num == -1 ||
        settings->server_ip == NULL ||
        settings->server_port == -1 ||
        settings->worker_threads == -1)
    {
        printf("You have not given all the parameters for this programm");
        return WRONG_INITIALIZATION_ERROR;
    }

    return SUCCESS;
}

void destroy_client_settings(ClientSettings *settings)
{
    if (settings->server_ip != NULL)
    {
        destroy_string(settings->server_ip);
    }
    if (settings->dir_name != NULL)
    {
        destroy_string(settings->dir_name);
    }
    free(settings);
}