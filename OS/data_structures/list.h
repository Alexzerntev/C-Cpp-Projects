#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MESSAGE_SIZE 100

typedef struct Container
{
    int sender;
    int reciever;
    char message[MESSAGE_SIZE];
} Container;

struct node
{
    Container data;
    int key;
    struct node *next;
    struct node *prev;
};

typedef struct Buffer
{
    struct node *head;
    struct node *current;
    struct node *last;
} Buffer;

void insert_first(int key, Container *data, Buffer *buffer);
struct node *delete_by_key(int key, Buffer *buffer);
struct node *find(int key, Buffer *buffer);
void delete_first(Buffer *buffer);
int isEmpty(Buffer *buffer);

#endif