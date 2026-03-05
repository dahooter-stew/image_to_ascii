#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

event_queue_node* create_event_queue_node(event e)
{
  event_queue_node* ptr = malloc(sizeof(event_queue_node));
  ptr->next = NULL;
  ptr->data = e;

  return ptr;
}

void delete_event_queue(event_queue_node* root)
{
  if (!root->next)
  {
    delete_event_queue(root->next);
  }

  free(root);
}

event_queue_node* event_enqueu(event_queue_node* root, event e)
{
  if (root == NULL) 
  {
    event_queue_node* temp = create_event_queue_node(e);
    return temp;
  }

  event_queue_node* curr = root;

  while(curr->next)
  {
    curr = curr->next;
  }

  curr->next = create_event_queue_node(e);

  return root;
}

event event_dequeue(event_queue_node** root)
{
  if((*root) == NULL)
    return (event){.type = INIT};

  event_queue_node* node = *root;
  *root = (*root)->next;

  event e = node->data;
  free(node);

  return e;
}

int event_queue_length(event_queue_node* root)
{
  int count = 0;

  while (root)
  {
    root = root->next;
    count++;
  }

  return count;
}


void print_event_queue(event_queue_node* root)
{
  while(root)
  {
    printf("%d, ", root->data.type);
    root = root->next;
  }
  printf("\n");
}
