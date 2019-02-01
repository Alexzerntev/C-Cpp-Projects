#include "../../../include/data_structures/posting_list/posting_list.hpp"

PostingList::PostingList()
{
    this->count = 0;
    this->posting_list_next_node = NULL;
}
PostingList::~PostingList()
{
}

void PostingList::increase_count()
{
    count++;
}
int PostingList::get_count()
{
    return this->count;
}