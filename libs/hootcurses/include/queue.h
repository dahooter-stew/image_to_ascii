#pragma once
#ifndef QUEUE
#define QUEUE

#define LIST_START 0
#define LIST_END -1

#include "types.h"

typedef struct event_queue_node
{
  struct event_queue_node* next;
  event data;
} event_queue_node;

event_queue_node*   create_event_queue_node(event e);
void                delete_event_queue(event_queue_node* root);

event_queue_node*   event_enqueu(event_queue_node* root, event e);
event               event_dequeue(event_queue_node** root);
int                 event_queue_length(event_queue_node* root);

void                print_event_queue(event_queue_node* root);

#endif
