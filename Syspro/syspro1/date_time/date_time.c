#include "./date_time.h"

int is_between_date_time(char *date1,
                         char *time1,
                         char *date2,
                         char *time2,
                         char *date3,
                         char *time3)
{
    char *date_time1, *date_time2, *date_time3;
    int result_code;

    date_time1 = concat_date_time(date1, time1, &result_code);
    if (result_code != SUCCESS)
    {
        return MEMORY_ALLOCATION_ERROR;
    }
    date_time2 = concat_date_time(date2, time2, &result_code);
    if (result_code != SUCCESS)
    {
        return MEMORY_ALLOCATION_ERROR;
    }
    date_time3 = concat_date_time(date3, time3, &result_code);
    if (result_code != SUCCESS)
    {
        return MEMORY_ALLOCATION_ERROR;
    }

    struct tm tm1 = {0};
    struct tm tm2 = {0};
    struct tm tm3 = {0};

    strptime(date_time1, "%d-%m-%Y-%H:%M", &tm1);
    strptime(date_time2, "%d-%m-%Y-%H:%M", &tm2);
    strptime(date_time3, "%d-%m-%Y-%H:%M", &tm3);

    // case before 1970
    time_t t1 = mktime(&tm1);
    if (t1 < 0)
    {
        t1 = 0;
    }
    time_t t2 = mktime(&tm2);
    if (t2 < 0)
    {
        t2 = 0;
    }
    time_t t3 = mktime(&tm3);
    if (t3 < 0)
    {
        t3 = 0;
    }

    destroy_string(date_time1);
    destroy_string(date_time2);
    destroy_string(date_time3);

    if (difftime(t3, t2) > 0 && difftime(t2, t1) > 0)
    {
        return 1;
    }
    return 0;
}

int compare_date_time(char *date1, char *time1, char *date2, char *time2)
{
    char *date_time1, *date_time2;
    int result_code;

    date_time1 = concat_date_time(date1, time1, &result_code);
    if (result_code != SUCCESS)
    {
        return MEMORY_ALLOCATION_ERROR;
    }
    date_time2 = concat_date_time(date2, time2, &result_code);
    if (result_code != SUCCESS)
    {
        return MEMORY_ALLOCATION_ERROR;
    }

    struct tm tm1 = {0};
    struct tm tm2 = {0};

    strptime(date_time1, "%d-%m-%Y-%H:%M", &tm1);
    strptime(date_time2, "%d-%m-%Y-%H:%M", &tm2);

    // case before 1970
    time_t t1 = mktime(&tm1);
    if (t1 < 0)
    {
        t1 = 0;
    }

    time_t t2 = mktime(&tm2);

    if (t2 < 0)
    {
        t2 = 0;
    }

    destroy_string(date_time1);
    destroy_string(date_time2);

    if (difftime(t2, t1) > 0)
    {
        return 1;
    }
    if (difftime(t2, t1) < 0)
    {
        return -1;
    }
    return 0;
}

char *get_now_date(int *result_code)
{
    int i;
    char *result;
    char buffer[100];
    time_t now = time(NULL);
    struct tm *time1 = localtime(&now);

    strftime(buffer, sizeof(buffer) - 1, "%d-%m-%Y", time1);
    result = (char *)malloc(11 * sizeof(char));

    if (result == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Get now date");
        return NULL;
    }

    result[10] = '\0';
    for (i = 0; i < 10; i++)
    {
        result[i] = buffer[i];
    }

    *result_code = SUCCESS;
    return result;
}

char *get_now_time(int *result_code)
{
    int i;
    char *result;
    char buffer[100];
    time_t now = time(NULL);
    struct tm *time1 = localtime(&now);

    strftime(buffer, sizeof(buffer) - 1, "%H:%M", time1);
    result = (char *)malloc(6 * sizeof(char));
    if (result == NULL)
    {
        *result_code = MEMORY_ALLOCATION_ERROR;
        print_error(MEMORY_ALLOCATION_ERROR, "Get now date");
        return NULL;
    }
    result[5] = '\0';
    for (i = 0; i < 5; i++)
    {
        result[i] = buffer[i];
    }

    *result_code = SUCCESS;
    return result;
}