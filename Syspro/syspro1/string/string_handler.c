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

char *copy_string(char *initial_string, int *result_code)
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

char *concat_date_time(const char *str1, const char *str2, int *result_code)
{
    int i;
    char *result = (char *)malloc(sizeof(char) * (17));

    if (result == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Concat date time");
        return NULL;
    }

    result[16] = '\0';
    for (i = 0; i < 10; i++)
    {
        result[i] = str1[i];
    }
    result[10] = '-';
    for (i = 0; i < 5; i++)
    {
        result[11 + i] = str2[i];
    }

    *result_code = SUCCESS;
    return result;
}

char *add_id_to_transaction(char *line, int id, int *result_code)
{
    int i;
    char buffer[12];
    sprintf(buffer, "%d", id);
    int id_size = strlen(buffer);
    int line_size = strlen(line);
    int size = id_size + line_size + 1;
    char *result = create_string(size, result_code);

    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Add id to transaction");
        return NULL;
    }

    for (i = 0; i < id_size; i++)
    {
        result[i] = buffer[i];
    }

    result[id_size] = ' ';

    for (i = 0; i < line_size; i++)
    {
        result[id_size + 1 + i] = line[i];
    }

    *result_code = SUCCESS;

    return result;
}

char *add_date_time_to_transaction(char *line, int *result_code)
{
    int i;
    int line_size = strlen(line);
    int total_size = line_size + 17;

    char *result = create_string(total_size, result_code);

    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Add date time to transaction");
        return NULL;
    }

    char *date = get_now_date(result_code);
    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Add date time to transaction");
        return NULL;
    }

    char *time = get_now_time(result_code);
    if (*result_code != SUCCESS)
    {
        print_error(*result_code, "Add date time to transaction");
        return NULL;
    }

    for (i = 0; i < line_size; i++)
    {
        result[i] = line[i];
    }

    result[line_size] = ' ';

    for (i = 0; i < 10; i++)
    {
        result[line_size + i + 1] = date[i];
    }

    result[line_size + 11] = ' ';

    for (i = 0; i < 5; i++)
    {
        result[line_size + i + 12] = time[i];
    }

    destroy_string(date);
    destroy_string(time);

    *result_code = SUCCESS;
    return result;
}

void remove_line_break(char *string_with_line_break)
{
    int i;

    for (i = 0; i < strlen(string_with_line_break); i++)
    {
        if (string_with_line_break[i] == '\n')
        {
            string_with_line_break[i] = '\0';
        }
    }

    return;
}

void remove_semicolon(char *string_with_line_break)
{
    int i;

    for (i = 0; i < strlen(string_with_line_break); i++)
    {
        if (string_with_line_break[i] == ';')
        {
            string_with_line_break[i] = '\0';
        }
    }

    return;
}

void destroy_string(char *string_to_be_destroyed)
{
    free(string_to_be_destroyed);
    return;
}