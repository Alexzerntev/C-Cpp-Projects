#include "../../include/string/path_list_node.hpp"

PathListNode::PathListNode(char *str, PathListNode *next)
{
    this->string = new char[strlen(str) + 1];
    if (this->string == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    strcpy(this->string, str);
    this->string[strlen(str)] = '\0';
    this->next = next;
}

PathListNode::~PathListNode()
{
    //delete[] (this->string);
}

void PathListNode::print()
{
    PathListNode *current = this;
    while (current != NULL)
    {
        printf("\nThe path is: %s\n", current->string);
        current = current->next;
    }
}