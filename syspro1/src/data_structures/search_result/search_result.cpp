#include "../../../include/data_structures/search_result/search_result.hpp"

SearchResult::SearchResult()
{
}

SearchResult::SearchResult(DocumentString *document_string, char *word, float score)
{
    this->document_string = document_string;
    this->word = new char[strlen(word)];
    strcpy(this->word, word);
    this->score = score;
}

SearchResult::~SearchResult()
{
    delete (this->word);
}

float SearchResult::get_score()
{
    return this->score;
}

void SearchResult::print(CommandString *command_string)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int console_line_length = w.ws_col;

    int document_string_length = this->document_string->get_length();
    int extra_space = 13 + this->get_digits(this->document_string->get_id());
    int total_length = document_string_length + extra_space;

    int times = total_length / console_line_length + 1;
    if (times > 1)
    {
        total_length = total_length - extra_space;
    }

    char *underline_string = new char[total_length];
    if (this->get_score() < 0)
    {
        extra_space++;
    }
    for (int i = 0; i < total_length; i++)
    {
        underline_string[i] = ' ';
    }
    int command_words_count = command_string->get_words_count();
    for (int i = 0; i < command_words_count; i++)
    {
        char *current_word = command_string->get_word_by_index(i);
        int word_length = strlen(current_word);
        int word_freq = this->document_string->get_frequancy(current_word);
        int current_pointer_index = 0;
        for (int j = 0; j < word_freq; j++)
        {
            char *ptr = strstr(this->document_string->string + current_pointer_index, current_word);
            if (ptr == NULL)
            {
                break;
            }
            current_pointer_index = ptr - this->document_string->string;
            if (current_pointer_index + word_length != this->document_string->get_length() && this->document_string->string[current_pointer_index + word_length] != ' ')
            {
                j--;
            }
            else
            {
                for (int k = 0; k < word_length; k++)
                {
                    if (times > 1)
                    {
                        underline_string[current_pointer_index + k] = '^';
                    }
                    else
                    {
                        underline_string[current_pointer_index + k + extra_space] = '^';
                    }
                }
            }
            current_pointer_index = ptr - this->document_string->string + 1;
        }
    }

    printf("\n(%d)[%5f] ", this->document_string->get_id(), this->get_score());
    if (times > 1)
    {
        printf("\n");
    }
    int last_j = 0;
    for (int i = 0; i < document_string_length; i++)
    {
        if (i % (console_line_length) == 0 && i != 0)
        {
            for (int j = last_j; j < i; j++)
            {
                printf("%c", underline_string[j]);
            }
            last_j = i;
            printf("%c", this->document_string->string[i]);
        }
        else
        {
            printf("%c", this->document_string->string[i]);
        }
    }
    printf("\n");

    for (int j = last_j; j < total_length; j++)
    {
        printf("%c", underline_string[j]);
    }

    delete (underline_string);
    printf("\n");
}

int SearchResult::get_digits(int number)
{
    if (number == 0)
    {
        return 1;
    }
    int count = 0;

    while (number != 0)
    {
        number /= 10;
        ++count;
    }
    return count;
}

int SearchResult::get_id()
{
    return this->document_string->get_id();
}