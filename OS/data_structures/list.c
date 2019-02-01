#include "list.h"

//insert link at the last location
//insert link at the first location
void insert_first(int key, Container *data, Buffer *buffer)
{
    Container cont;
    memcpy(&cont, data, sizeof(Container));
    //create a link
    struct node *link = (struct node *)malloc(sizeof(struct node));
    link->key = key;
    link->data = cont;

    if (isEmpty(buffer))
    {
        //make it the last link
        buffer->last = link;
    }
    else
    {
        //update first prev link
        buffer->head->prev = link;
    }

    //point it to old first link
    link->next = buffer->head;

    //point first to new first link
    buffer->head = link;
}

//delete a link with given key

struct node *delete_by_key(int key, Buffer *buffer)
{

    //start from the first link
    struct node *current = buffer->head;
    struct node *previous = NULL;

    //if list is empty
    if (buffer->head == NULL)
    {
        return NULL;
    }

    //navigate through list
    while (current->key != key)
    {
        //if it is last node

        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            //store reference to current link
            previous = current;

            //move to next link
            current = current->next;
        }
    }

    //found a match, update the link
    if (current == buffer->head)
    {
        //change first to point to next link
        buffer->head = buffer->head->next;
    }
    else
    {
        //bypass the current link
        current->prev->next = current->next;
    }

    if (current == buffer->last)
    {
        //change last to point to prev link
        buffer->last = current->prev;
    }
    else
    {
        current->next->prev = current->prev;
    }

    return current;
}

//find a link with given key
struct node *find(int key, Buffer *buffer)
{

    //start from the first link
    struct node *current = buffer->head;

    //if list is empty
    if (buffer->head == NULL)
    {
        return NULL;
    }

    //navigate through list
    while (current->key != key)
    {

        //if it is last node
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            //go to next link
            current = current->next;
        }
    }

    //if data found, return the current Link
    return current;
}

//delete first item
void delete_first(Buffer *buffer)
{

    //save reference to first link
    struct node *tempLink = buffer->head;

    //mark next to first link as first
    buffer->head = buffer->head->next;

    //return the deleted link
    free(tempLink);
}

//is list empty
int isEmpty(Buffer *buffer)
{
    return buffer->head == NULL;
}