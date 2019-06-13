#include "./error_handler.h"

void print_error(int error_code, const char *message)
{
    if (error_code == MEMORY_ALLOCATION_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to allocate memory\n");
        perror("malloc");
        return;
    }
    else if (error_code == FLOAT_CONVERTION_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to convert string to float\n");
        perror("atof");
        return;
    }
    else if (error_code == INT_CONVERTION_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to convert string to int\n");
        perror("atoi");
        return;
    }
    else if (error_code == WRONG_INITIALIZATION_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to initialize the app\n");
        return;
    }
    else if (error_code == STRING_PARSING_ERROR)
    {
        printf("%s\n", message);
        printf("Failed to parse string\n");
        return;
    }
    else if (error_code == DUPLICATE_USER_ERROR)
    {
        printf("%s\n", message);
        printf("User already exists\n");
        return;
    }
    else if (error_code == USER_NOT_FOUND_ERROR)
    {
        printf("%s\n", message);
        printf("User not found\n");
        return;
    }
    else if (error_code == NOT_ENOUGH_MONEY_ERROR)
    {
        printf("%s\n", message);
        printf("Not enough money to execute transaction\n");
        return;
    }
    else if (error_code == BITCOIN_NOT_FOUND_ERROR)
    {
        printf("%s\n", message);
        printf("Bitcoin not found\n");
        return;
    }
    else if (error_code == WRONG_INPUT)
    {
        printf("%s\n", message);
        printf("Wrong Input\n");
        return;
    }
    else
    {
        printf("%s\n", message);
    }
}