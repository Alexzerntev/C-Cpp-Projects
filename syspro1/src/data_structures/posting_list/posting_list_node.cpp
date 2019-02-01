#include "../../../include/data_structures/posting_list/posting_list_node.hpp"

PostingListNode::PostingListNode(int id, int frequency)
{
    this->id = id;
    this->frequency = frequency;
    this->next = NULL;
}
PostingListNode::~PostingListNode()
{
    delete (this->next);
}
int PostingListNode::get_id()
{
    return this->id;
}
int PostingListNode::get_frequency()
{
    return this->frequency;
}