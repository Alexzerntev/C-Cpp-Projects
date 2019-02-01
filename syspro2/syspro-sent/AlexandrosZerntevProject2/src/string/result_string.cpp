#include "../../include/string/result_string.hpp"

ResultString::ResultString(char *str, ResultString *next, orderType type)
{
    this->next = next;
    unsigned int counter = 0;
    char *token;
    this->string = new char[strlen(str) + 1];
    if (this->string == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    strcpy(this->string, str);
    this->string[strlen(str)] = '\0';
    this->type = type;
    // token = strtok(this->string, ":");

    // while (token != NULL)
    // {
    //     printf("token is %s\n", token);

    //     token = strtok(NULL, ":");
    // }

    // if (strcmp(token, "/search") == 0)
    // {
    //     this->type = search_type;
    //     /* walk through other tokens */
    // }
    // else if (strcmp(token, "/maxcount") == 0)
    // {
    //     this->type = maxcount_type;
    // }
    // else if (strcmp(token, "/mincount") == 0)
    // {
    //     this->type = mincount_type;
    // }
    // else if (strcmp(token, "/wc") == 0)
    // {
    //     this->type = workercount_type;
    // }
    // else if (strcmp(token, "/path_distro") == 0)
    // {
    //     this->type = path_distribution_type;
    // }
    // else if (strcmp(token, "/exit") == 0)
    // {
    //     this->type = exit_type;
    // }
    // else
    // {
    //     this->type = wrong_input;
    // }
}

ResultString::~ResultString()
{
    if (this->next != NULL)
    {
        delete (this->next);
    }
    delete[](this->string);
}

orderType ResultString::get_type()
{
    return this->type;
}

void ResultString::set_data()
{
    if (this->type == mincount_type || this->type == maxcount_type)
    {
        char *token;
        token = strtok(this->string, " ");
        int count = 1;
        while (token != NULL)
        {
            if (count == 3)
            {
                this->minimax = atoi(token);
            }
            token = strtok(NULL, " ");
            count++;
        }
    }
    if (this->type == workercount_type)
    {
        char *token;
        token = strtok(this->string, " ");
        int count = 1;
        while (token != NULL)
        {
            if (count == 3)
            {
                this->lines_count = atoi(token);
            }
            if (count == 4)
            {
                this->words_count = atoi(token);
            }
            if (count == 5)
            {
                this->bytes_count = atoi(token);
            }
            token = strtok(NULL, " ");
            count++;
        }
    }
}