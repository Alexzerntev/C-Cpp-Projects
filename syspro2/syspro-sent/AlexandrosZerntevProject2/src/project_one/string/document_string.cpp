#include "../../../include/project_one/string/document_string.hpp"

DocumentString::DocumentString(char *string, int idx)
{
    this->index = idx;
    this->set_string(string);
}
DocumentString::~DocumentString()
{
}

int DocumentString::get_index()
{
    return this->index;
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