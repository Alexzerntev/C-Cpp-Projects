#include "../../include/string/command_string.hpp"

CommandString::CommandString(char *string)
{
    string[strlen(string) - 1] = '\0';
    if (strcmp(string, "/df") == 0 && strlen(string) == 3)
    {
        this->type = all_document_frequancy_type;
        return;
    }
    if (strcmp(string, "exit") == 0 && strlen(string) == 4)
    {
        this->type = exit_type;
        return;
    }

    int counter = 0;
    while (string[counter] != ' ' && counter < strlen(string))
    {
        counter++;
    }

    // seperate the command
    char *command = new char[counter + 1];
    command[counter] = '\0';
    memcpy(command, string, counter);
    // seperate the string
    int actual_string_length = strlen(string) - counter;
    if (actual_string_length != 0)
    {
        char actual_string[actual_string_length];
        actual_string[actual_string_length - 1] = '\0';
        memcpy(actual_string, string + counter + 1, actual_string_length - 1);
        this->set_string(actual_string);
    }

    if (strcmp(command, "/search") == 0)
    {
        this->type = search_type;
    }
    else if (strcmp(command, "/df") == 0 && actual_string_length != 0)
    {
        this->type = word_document_frequancy_type;
    }
    else if (strcmp(command, "/tf") == 0)
    {
        this->type = term_frequancy_type;
    }
    else
    {
        this->type = wrong_input;
    }

    delete (command);
}
CommandString::~CommandString()
{
}

commandType CommandString::get_type()
{
    return this->type;
}