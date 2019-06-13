#include "./string_handler.h"

char *create_string(int size, int *result_code)
{
    int i;
    char *result = (char *)malloc((size + 1) * sizeof(char));

    if (result == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    for (i = 0; i < size; i++)
    {
        result[i] = '-';
    }

    result[size] = '\0';
    *result_code = SUCCESS;
    return result;
}

int index_of(char *string, char c)
{
    char *e;
    int index;

    e = strchr(string, c);
    index = (int)(e - string);
    return index;
}

char *copy_string(char *initial_string, int *result_code)
{
    int size = strlen(initial_string) + 1;
    char *new_string = (char *)malloc(size * sizeof(char));

    if (new_string == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "String copy");
        *result_code = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    strcpy(new_string, initial_string);
    new_string[size - 1] = '\0';
    *result_code = SUCCESS;
    return new_string;
}

char *copy_const_string(const char *initial_string, int *result_code)
{
    int size = strlen(initial_string) + 1;
    char *new_string = (char *)malloc(size * sizeof(char));

    if (new_string == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    strcpy(new_string, initial_string);
    new_string[size - 1] = '\0';
    *result_code = SUCCESS;
    return new_string;
}

void remove_linebreak(char *string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == '\n')
        {
            string[i] = '\0';
        }
    }
}

void destroy_string(char *string_to_be_destroyed)
{
    free(string_to_be_destroyed);
    return;
}