#include "../../../../include/project_one/data_structs/map/map.hpp"

Map::Map(char *filename)
{
    this->entry_count = 0;
    this->word_count = 0;
    this->bytes_count = 0;
    this->filename = new char[strlen(filename)];
    if (this->filename == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    strncpy(this->filename, filename, strlen(filename));
    this->filename[strlen(this->filename) - 2] = '\0';
    this->trie = new TrieNode(-1, NULL);
    if (this->trie == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
}

Map::~Map()
{
    for (int i = 0; i < this->entry_count; i++)
    {
        delete (entries[i]);
    }
    delete (filename);
    delete (trie);
}

result Map::execute_mapping()
{
    if (!this->find_lines_count())
    {
        return error;
    }

    this->entries = new DocumentString *[this->entry_count];
    if (this->entries == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }

    if (!this->load_lines())
    {
        return error;
    }

    return success;
}

result Map::find_lines_count()
{

    FILE *file = fopen(this->filename, "r");

    char *buffer = NULL;
    size_t length;
    if (file == NULL)
    {
        printf("\ERROR opening file! %d #%s# \n", this->filename, errno);
        return error;
    }

    while (getline(&buffer, &length, file) != EOF)
    {
        this->entry_count++;
    }
    fclose(file);

    return success;
}

result Map::load_lines()
{
    FILE *file = fopen(this->filename, "r");
    if (file == NULL)
    {
        printf("\nError while loading document lines \n");
        return error;
    }
    size_t length;
    char *buffer = NULL;
    int current_line = 0;
    unsigned long int sum_of_words = 0;
    while (getline(&buffer, &length, file) != EOF)
    {
        this->entries[current_line] = new DocumentString(buffer, current_line);
        if (this->entries[current_line] == NULL)
        {
            printf("errno = % d \ n ", errno);
            perror("malloc");
        }
        //printf("\nCurrently at line : %d", current_line);
        int words_count = this->entries[current_line]->get_words_count();
        sum_of_words += words_count;

        DocumentString *temp_doc_string = new DocumentString(buffer, current_line);
        if (temp_doc_string == NULL)
        {
            printf("errno = % d \ n ", errno);
            perror("malloc");
        }

        char *word = strtok(temp_doc_string->string, " ");
        if (word == NULL)
        {
            printf("\nError finding the word by index\n");
            return error;
        }
        PostingListNode *posting_list_node = new PostingListNode(
            this->entries[current_line]->get_index(), this->entries[current_line]->get_frequancy(word));
        if (posting_list_node == NULL)
        {
            printf("errno = % d \ n ", errno);
            perror("malloc");
        }
        this->trie->add_word_by_char(word, posting_list_node);

        //delete (word);

        while (word != NULL)
        {
            this->bytes_count += strlen(word);
            if (word == NULL)
            {
                printf("\nError finding the word by index\n");
                return error;
            }
            PostingListNode *posting_list_node = new PostingListNode(
                this->entries[current_line]->get_index(), this->entries[current_line]->get_frequancy(word));
            if (posting_list_node == NULL)
            {
                printf("errno = % d \ n ", errno);
                perror("malloc");
            }
            this->trie->add_word_by_char(word, posting_list_node);
            //delete (word);
            word = strtok(NULL, " ");
        }
        current_line++;
        delete (temp_doc_string);
    }
    fclose(file);
    this->avgdl = sum_of_words / this->entry_count;
    this->word_count = sum_of_words;
    return success;
}

int Map::get_entry_count()
{
    return this->entry_count;
}

int Map::get_words_count()
{
    return this->word_count;
}

int Map::get_bytes_count()
{
    return this->bytes_count;
}

int Map::get_word_frequancy(char *word) // to get frequancy in whole map
{
    return this->trie->get_frequancy(word);
}

int Map::get_word_document_frequancy(char *word, int id) // to get frequancy in specific entry
{
    return this->entries[id]->get_frequancy(word);
}

void Map::print_strings()
{
    for (int i = 0; i < this->entry_count; i++)
    {
        this->entries[i]->print();
    }
}

PostingList *Map::get_words_posting_list(char *word)
{
    return trie->get_words_posting_list(word);
}

double Map::get_entry_avgdl()
{
    return this->avgdl;
}