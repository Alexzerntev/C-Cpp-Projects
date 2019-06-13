#include "./settings.h"

Settings *create_settings(int *result_code)
{
    Settings *settings = (Settings *)malloc(sizeof(Settings));
    if (settings == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Could not create settings");
    }

    settings->buffer_size = -1;
    settings->common_dir = NULL;
    settings->log_file = NULL;
    settings->mirror_dir = NULL;
    settings->id = -1;
    settings->input_dir = NULL;

    return settings;
}

int validate_and_store_settings(char **console_input, int size, Settings *settings)
{
    int result_code = SUCCESS;
    if (size != 13)
    {
        return WRONG_INITIALIZATION_ERROR;
    }

    int i;
    for (i = 1; i < size; i++)
    {
        if (strcmp(console_input[i], "-n") == 0)
        {
            i++;
            settings->id = atoi(console_input[i]);

            if (settings->id == 0 && strlen(console_input[i]) != 1)
            {
                print_error(INT_CONVERTION_ERROR, "Wrong id");
                return INT_CONVERTION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-c") == 0)
        {
            i++;
            settings->common_dir = copy_string(console_input[i], &result_code);

            if (result_code != SUCCESS)
            {
                print_error(result_code, "Common directory");
                return result_code;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-i") == 0)
        {
            i++;
            settings->input_dir = copy_string(console_input[i], &result_code);

            if (result_code != SUCCESS)
            {
                print_error(result_code, "Input directory");
                return result_code;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-m") == 0)
        {
            i++;

            settings->mirror_dir = copy_string(console_input[i], &result_code);

            if (result_code != SUCCESS)
            {
                print_error(result_code, "Mirror directory");
                return result_code;
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

            if (settings->buffer_size < 10)
            {
                print_error(INT_CONVERTION_ERROR, "Buffer size is too small");
                return INT_CONVERTION_ERROR;
            }

            continue;
        }
        else if (strcmp(console_input[i], "-l") == 0)
        {
            i++;

            settings->log_file = copy_string(console_input[i], &result_code);

            if (result_code != SUCCESS)
            {
                print_error(result_code, "Log file");
                return result_code;
            }

            continue;
        }
        else
        {
            return WRONG_INITIALIZATION_ERROR;
        }
    }

    return SUCCESS;
}

void destroy_settings(Settings *settings)
{
    if (settings->common_dir != NULL)
    {
        destroy_string(settings->common_dir);
    }
    if (settings->input_dir != NULL)
    {
        destroy_string(settings->input_dir);
    }
    if (settings->log_file != NULL)
    {
        destroy_string(settings->log_file);
    }
    if (settings->mirror_dir != NULL)
    {
        destroy_string(settings->mirror_dir);
    }
    free(settings);
}