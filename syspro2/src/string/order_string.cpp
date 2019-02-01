#include "../../include/string/order_string.hpp"

OrderString::OrderString(char *str)
{
    unsigned int counter = 0;
    this->deadline = 0;
    this->string = NULL;
    while (str[counter] != ' ' && counter < strlen(str))
    {
        counter++;
    }

    // seperate the command
    char *command = new char[counter + 1];
    if (command == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    memcpy(command, str, counter);
    command[counter] = '\0';
    // seperate the string
    char *actual_string = new char[strlen(str) - counter + 1];
    if (actual_string == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    memcpy(actual_string, str + counter + 1, strlen(str) - counter);
    this->set_string(actual_string);

    if (strcmp(command, "/search") == 0 && strlen(actual_string) != 0 && strstr(actual_string, "-d") != NULL)
    {
        this->deadline = atoi(strstr(actual_string, "-d") + 3);
        this->type = search_type;
    }
    else if (strcmp(command, "/maxcount") == 0 && strlen(actual_string) != 0)
    {
        this->type = maxcount_type;
    }
    else if (strcmp(command, "/mincount") == 0 && strlen(actual_string) != 0)
    {
        this->type = mincount_type;
    }
    else if (strcmp(command, "/wc") == 0)
    {
        this->type = workercount_type;
    }
    else if (strcmp(command, "/path_distro") == 0 && strlen(actual_string) != 0)
    {
        this->type = path_distribution_type;
    }
    else if (strcmp(command, "/exit") == 0)
    {
        this->type = exit_type;
    }
    else
    {
        this->type = wrong_input;
    }
    delete[](command);
    delete[](actual_string);
}

OrderString::~OrderString()
{
    delete (this->string);
}

orderType OrderString::get_type()
{
    return this->type;
}

char *OrderString::get_string()
{
    return this->string;
}

void OrderString::set_string(char *str)
{
    this->string = new char[strlen(str) + 1];
    if (this->string == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    strncpy(this->string, str, strlen(str));
    this->string[strlen(str)] = '\0';
}

int OrderString::get_deadline()
{
    return this->deadline;
}

char *OrderString::get_word_by_index(int index)
{
    int count = 0, starts = -1, ends = -1;
    int length = strlen(this->string);
    for (int i = 0; i < length; i++)
    {
        if (count == index && starts == -1)
        {
            starts = i;
        }
        if ((string[i] == ' ' || i == length - 1) && starts != -1 && ends == -1)
        {
            if (i == length - 1)
            {
                ends = i;
            }
            else
            {
                ends = i - 1;
            }
            char *word = new char[ends - starts + 1];
            if (word == NULL)
            {
                printf("errno = % d \ n ", errno);
                perror("malloc");
            }
            word[ends - starts + 1] = '\0';
            memcpy(word, this->string + starts, ends - starts + 1);
            return word;
        }
        if (string[i] == ' ')
        {
            count++;
        }
    }
    return NULL;
}

int OrderString::get_words_count()
{
    int count = 1;
    for (int i = 0; i < strlen(this->string); i++)
    {
        if (string[i] == ' ')
        {
            count++;
        }
    }
    return count;
}