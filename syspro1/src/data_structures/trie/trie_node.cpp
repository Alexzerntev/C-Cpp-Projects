#include "../../../include/data_structures/trie/trie_node.hpp"

TrieNode::TrieNode(char c, PostingList *posting_list)
{
    this->posting_list = posting_list;
    this->content = c;
    this->next_horizontal = NULL;
    this->next_vertical = NULL;
}

TrieNode::~TrieNode()
{
    delete (this->next_horizontal);
    delete (this->next_vertical);
    delete (this->posting_list);
}

char TrieNode::get_content()
{
    return this->content;
}

void TrieNode::set_content(char c)
{
    this->content = c;
}

result TrieNode::add_word_by_char(
    char *word,
    PostingListNode *posting_list_node)
{
    int index = 0;
    bool flag = false;
    TrieNode *current_horizontal_node = this;
    TrieNode *previus_horizontal_node = NULL;
    if (current_horizontal_node->get_content() == -1) // This is root case
    {
        current_horizontal_node->set_content(word[index]);
        if (index == strlen(word) - 1) // this is the case root is only one character
        {
            return success;
        }
        while (index != strlen(word) - 1) // complete the res of the word
        {
            index++;
            current_horizontal_node->next_vertical = new TrieNode(word[index]);
            current_horizontal_node = current_horizontal_node->next_vertical;
        }
        current_horizontal_node->posting_list = new PostingList();
        current_horizontal_node->posting_list->increase_count();
        current_horizontal_node->posting_list->posting_list_next_node = posting_list_node;
        return success;
    }

    while (current_horizontal_node != NULL) // search horizontally
    {
        if (word[index] != current_horizontal_node->get_content())
        {
            previus_horizontal_node = current_horizontal_node;
            current_horizontal_node = current_horizontal_node->next_horizontal;
            continue;
        }
        // if found go to the next letter
        index++;
        previus_horizontal_node = current_horizontal_node;
        current_horizontal_node = current_horizontal_node->next_vertical;
        if (current_horizontal_node == NULL)
        {
            flag = true;
        }
    }
    if (index == strlen(word)) // word already exists
    {
        if (previus_horizontal_node->posting_list == NULL)
        {
            previus_horizontal_node->posting_list = new PostingList();
            previus_horizontal_node->posting_list->posting_list_next_node = posting_list_node;
            return success;
        }
        if (this->id_exists(previus_horizontal_node->posting_list, posting_list_node->get_id()))
        {
            return success;
        }
        PostingListNode *temp = previus_horizontal_node->posting_list->posting_list_next_node;
        previus_horizontal_node->posting_list->posting_list_next_node = posting_list_node;
        previus_horizontal_node->posting_list->posting_list_next_node->next = temp;
        previus_horizontal_node->posting_list->increase_count();
        return success;
    }
    // if not found
    current_horizontal_node = new TrieNode(word[index]);
    if (flag)
    {
        previus_horizontal_node->next_vertical = current_horizontal_node;
    }
    else
    {
        previus_horizontal_node->next_horizontal = current_horizontal_node;
    }
    while (index != strlen(word) - 1) // complete the res of the word
    {
        index++;
        current_horizontal_node->next_vertical = new TrieNode(word[index]);
        current_horizontal_node = current_horizontal_node->next_vertical;
    }
    current_horizontal_node->posting_list = new PostingList();
    current_horizontal_node->posting_list->increase_count();
    current_horizontal_node->posting_list->posting_list_next_node = posting_list_node;
    return success;
}

PostingList *TrieNode::get_words_posting_list(char *word)
{
    int index = 0;
    TrieNode *current_horizontal_node = this;
    TrieNode *previus_horizontal_node = NULL;
    while (current_horizontal_node != NULL) // search horizontally
    {
        if (word[index] != current_horizontal_node->get_content())
        {
            previus_horizontal_node = current_horizontal_node;
            current_horizontal_node = current_horizontal_node->next_horizontal;
            continue;
        }
        // if found go to the next letter
        index++;
        previus_horizontal_node = current_horizontal_node;
        current_horizontal_node = current_horizontal_node->next_vertical;
        if (index == strlen(word) && previus_horizontal_node->posting_list != NULL)
        {
            return previus_horizontal_node->posting_list;
        }
    }
    return NULL;
}

int TrieNode::get_frequancy(char *word)
{
    int index = 0;
    TrieNode *current_horizontal_node = this;
    TrieNode *previus_horizontal_node = NULL;
    while (current_horizontal_node != NULL) // search horizontally
    {
        if (word[index] != current_horizontal_node->get_content())
        {
            previus_horizontal_node = current_horizontal_node;
            current_horizontal_node = current_horizontal_node->next_horizontal;
            continue;
        }
        // if found go to the next letter
        index++;
        previus_horizontal_node = current_horizontal_node;
        current_horizontal_node = current_horizontal_node->next_vertical;
        if (index == strlen(word) && previus_horizontal_node->posting_list != NULL)
        {
            return previus_horizontal_node->posting_list->get_count();
        }
    }

    return -1;
}

void TrieNode::print_all_trie_frequancies()
{
    if (this == NULL)
    {
        printf("\nThere is no existing map\n");
        return;
    }
    Stack *stack = new Stack();
    TrieNode *current_trie_node = this;
    stack->push(current_trie_node);
    do
    {
        if (current_trie_node->next_vertical != NULL)
        {
            current_trie_node = current_trie_node->next_vertical;
            stack->push(current_trie_node);
            continue;
        }
        if (current_trie_node->posting_list != NULL)
        {
            stack->print();
        }
        if (current_trie_node->next_horizontal != NULL)
        {
            stack->pop();
            current_trie_node = current_trie_node->next_horizontal;
            continue;
        }
        do
        {
            current_trie_node = stack->pop();

        } while (current_trie_node->next_horizontal == NULL && !stack->is_empty());

        if (current_trie_node->next_horizontal != NULL)
        {
            current_trie_node = current_trie_node->next_horizontal;
            stack->push(current_trie_node);
        }
    } while (!stack->is_empty());
}

bool TrieNode::id_exists(PostingList *posting_list, int id)
{
    PostingListNode *current_posting_node = posting_list->posting_list_next_node;
    while (current_posting_node != NULL)
    {
        if (current_posting_node->get_id() == id)
        {
            posting_list->increase_count();
            return true;
        }
        current_posting_node = current_posting_node->next;
    }
    return false;
}