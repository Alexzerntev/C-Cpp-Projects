#include "../../include/string/map_list_node.hpp"

MapListNode::MapListNode(char *string, MapListNode *next)
{
    this->map = new Map(string);
    if (this->map == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
    this->map->execute_mapping();
    this->next = next;
}

MapListNode::~MapListNode()
{
    //delete (this->map);
}

void MapListNode::print()
{
}