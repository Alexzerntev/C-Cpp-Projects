#include "../include/user_interaction.hpp"

UserInteraction::UserInteraction()
{
    this->result_count = 5;
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
    if (strcmp(arguments[1], "-i") != 0 || strcmp(arguments[3], "-k") != 0)
    {
        printf("Wrong sequence of arguments \n");
        return false;
    }
    this->result_count = atoi(arguments[4]);
    return true;
}

result UserInteraction::interact()
{
    size_t length;
    char *buffer = NULL;
    int current_line = 0;
    while (true)
    {
        printf("\nGive me a command to execute\n");
        getline(&buffer, &length, stdin);
        CommandString *command_string = new CommandString(buffer);
        //CommandString *command_string = new CommandString("/df");
        commandType type = command_string->get_type();

        switch (type)
        {
        case search_type: // /search
        {
            if (command_string->get_words_count() == 0)
            {
                printf("\nYou didn't give a word to search\n");
                break;
            }
            this->search(command_string);
            break;
        }
        case all_document_frequancy_type: // /df case
        {
            this->map->print_all_trie_frequancy();
            break;
        }
        case word_document_frequancy_type: // /df case + word
        {
            if (command_string->get_words_count() > 1)
            {
                printf("\n/df can be used only with one word\n");
                break;
            }
            char *word = command_string->get_word_by_index(0);
            int result = this->map->get_word_frequancy(word);
            if (result == -1)
            {
                printf("\nThe word not found\n");
                delete (word);
                break;
            }
            printf("\n%s %d\n", word, result);
            delete (word);
            break;
        }
        case term_frequancy_type: // /tf
        {
            if (command_string->get_words_count() != 2)
            {
                printf("\nYou didn't give the proper amount of arguments\n");
                break;
            }
            char *word = command_string->get_word_by_index(1);
            int id = atoi(command_string->get_word_by_index(0));
            if (id >= this->map->get_entry_count() || id < 0)
            {
                printf("\nId out of bound\n");
                break;
            }
            int freq = this->map->get_word_document_frequancy(word, id);
            if (freq == 0)
            {
                printf("\nWord not found\n");
                break;
            }
            printf("\n%d %s %d\n", id, word, freq);
            delete (word);
            break;
        }
        case exit_type: // exit
        {
            return success;
        }
        case wrong_input: // wrond input
        {
            printf("\nWrong Input\n");
            break;
        }
        }
        delete (command_string);
        //break; // to be deleted
    }
}

void UserInteraction::search(CommandString *command_string)
{
    Heap *heap = new Heap(this->result_count, command_string);
    for (int i = 0; i < command_string->get_words_count(); i++)
    {
        char *word = command_string->get_word_by_index(i);
        PostingList *posting_list = this->map->get_words_posting_list(word);
        if (posting_list == NULL)
        {
            printf("\nThe word '%s' does not exist in the map\n", word);
            return;
        }

        PostingListNode *current_posting_list_node = posting_list->posting_list_next_node;

        while (current_posting_list_node != NULL)
        {
            if (heap->id_exists(current_posting_list_node->get_id()))
            {
                current_posting_list_node = current_posting_list_node->next;
                continue;
            }
            SearchResult *search_result = new SearchResult(
                this->map->entries[current_posting_list_node->get_id()],
                word,
                get_score(command_string, word, current_posting_list_node->get_id()));

            heap->add(search_result);
            current_posting_list_node = current_posting_list_node->next;
        }
    }
    heap->print_results();
    delete (heap);
}

float UserInteraction::get_score(CommandString *command_string, char *word, int id)
{
    float score = 0;
    score = this->get_idf(command_string) * this->get_coefficient(word, id);
    return score;
}

float UserInteraction::get_idf(CommandString *command_string)
{
    float idf = 0;
    int N = map->get_entry_count();
    int n = 0;
    for (int i = 0; i < command_string->get_words_count(); i++)
    {
        char *word = command_string->get_word_by_index(i);
        n = map->get_word_frequancy(word);
        idf = idf + log10((N - n + 0.5) / (n + 0.5));
        delete (word);
    }
    return idf;
}

float UserInteraction::get_coefficient(char *word, int id)
{
    float coefficient = 0;
    int words_freq = this->map->entries[id]->get_frequancy(word);
    int words_count = this->map->entries[id]->get_words_count();

    coefficient = (words_freq * (k + 1)) / (words_freq + (k * (1 - b + (b * (words_count / this->map->get_entry_avgdl())))));

    return coefficient;
}

void UserInteraction::set_map(Map *map)
{
    this->map = map;
}
