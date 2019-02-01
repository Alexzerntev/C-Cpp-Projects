#include "../../include/string/string.hpp"

String::String(char *string)
{
    this->normalize(string);
    this->string = new char[strlen(string) + 1];
    this->string[strlen(string) + 1] = '\0';
    strncpy(this->string, string, strlen(string));
}
String::String()
{
}
String::~String()
{
}

int String::get_length()
{
    return strlen(this->string);
}

void String::set_string(char *string)
{
    this->normalize(string);
    this->string = new char[strlen(string) + 1];
    this->string[strlen(string)] = '\0';
    strncpy(this->string, string, strlen(string));
}

int String::get_words_count()
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

char *String::get_word_by_index(int index)
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

void String::normalize(char *string)
{
    int current = 0;
    for (int i = 0; string[i] != '\0'; ++i)
        if (!isspace(string[i]) || (i > 0 && !isspace(string[i - 1])))
        {
            string[current++] = string[i];
        }
    string[current] = '\0';
}

void String::print()
{
    printf("%s\n", this->string);
}