#include "../include/user_interaction.hpp"

UserInteraction::UserInteraction()
{
}

UserInteraction::~UserInteraction()
{
}

bool UserInteraction::validate(int argument_count, char *arguments[])
{
    if (argument_count != 5)
    {
        printf("You didn't give the proper amount of arguments \n");
        return false;
    }
    if (strcmp(arguments[1], "-d") != 0 || strcmp(arguments[3], "-w") != 0)
    {
        printf("Wrong sequence of arguments \n");
        return false;
    }
    return true;
}

char *UserInteraction::interact()
{
    //return "/wc";
    //return "/search test -d 5";
    size_t length;
    char *buffer = NULL;
    int current_line = 0;
    printf("\nGive me a command to execute\n");
    getline(&buffer, &length, stdin);
    char *str = new char[strlen(buffer)];
    if (str == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    strcpy(str, buffer);
    str[strlen(buffer) - 1] = '\0';
    return str;
}