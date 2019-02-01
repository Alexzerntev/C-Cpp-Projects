#include "../../../../include/project_one/data_structs/posting_list/posting_list_node.hpp"

PostingListNode::PostingListNode(int index, int frequency)
{
    this->index = index;
    this->frequency = frequency;
    this->next = NULL;
}
PostingListNode::~PostingListNode()
{
    delete (this->next);
}
int PostingListNode::get_index()
{
    return this->index;
}
int PostingListNode::get_frequency()
{
    return this->frequency;
}