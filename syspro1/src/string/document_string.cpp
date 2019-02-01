#include "../../include/string/document_string.hpp"

DocumentString::DocumentString(char *string)
{
    for (int i = 0; i < strlen(string) - 1; i++)
    {
        if (string[i] == '\t' || string[i] == '.' || string[i] == ',')
        {
            string[i] = ' ';
        }
    }

    int counter = 0;
    while (string[counter] != ' ' && counter < strlen(string))
    {
        counter++;
    }
    if (counter == strlen(string))
    {
        printf("\nThere is no spaceses in the string '%s'\n", string);
        return;
    }

    // seperate the id
    char id[counter];
    memcpy(id, string, counter);
    this->id = atoi(id);

    // seperate the id
    int actual_string_length = strlen(string) - counter;
    if (strcmp(string + strlen(string) - 2, "\r\n") == 0)
    {
        actual_string_length = actual_string_length - 2;
    }
    else
    {
        int index = strlen(string) - 1;
        while (string[index] == '\n' || string[strlen(string) - 1] == ' ')
        {
            string[index] = '\0';
            index--;
        }
    }

    char actual_string[actual_string_length];
    actual_string[actual_string_length - 1] = '\0';
    memcpy(actual_string, string + counter + 1, actual_string_length - 1);
    this->set_string(actual_string);
}
DocumentString::~DocumentString()
{
}

int DocumentString::get_id()
{
    return this->id;
}

int DocumentString::get_frequancy(char *word)
{
    int count = 0;
    int current_pointer_index = 0;
    char *current_word;
    do
    {
        current_word = strstr(this->string + current_pointer_index, word);
        current_pointer_index = current_word - this->string;
        if (current_word != NULL)
        {
            count++;
            current_pointer_index++;
        }
    } while (current_word != NULL);
    return count;
}