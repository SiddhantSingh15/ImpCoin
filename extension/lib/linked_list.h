#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>

typedef struct node {
  void *value;
  struct node *next;
} node;

typedef struct linked_list {
  node *head;
  uint32_t size;
} linked_list;

linked_list *init_linked_list(void);

node *init_node(void *val);

void append_to_linked_list(linked_list *list, void *val);

void free_node(node *to_free, void (*value_free)(void *));

void delete_node(node *to_delete, linked_list *list,
                 void (*value_free)(void *));

void free_linked_list(linked_list *list, void (*value_free)(void *));

#endif
