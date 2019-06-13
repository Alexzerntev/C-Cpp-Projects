#include "./helpers.h"

int get_nuber_of_digits(int number)
{
    int count = 0;

    while (number != 0)
    {
        number /= 10;
        ++count;
    }
    return count;
}

unsigned int get_file_size(char *file_name, int *result_code)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("Could not open file to get file size : %s\n", file_name);
        *result_code = COULD_NOT_OPEN_FILE;
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    unsigned int size = ftell(fp);
    fclose(fp);
    return size;
}

