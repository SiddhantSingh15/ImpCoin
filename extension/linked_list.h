#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>

typedef struct node {
  void *value;
  node *next;
} node;

typedef struct linked_list {
  node *head;
  uint32_t size;
} linked_list;

linked_list *init_linked_list(void);

node *init_node(void *val);

uint32_t append_via_node(node *entry, void *val);

uint32_t append_to_linked_list(linked_list *list, void *val);

node *traverse_linked_list(linked_list *list);

void change_node(linked_list *list, void *val);

void free_linked_list(linked_list *list, void (*value_free)(void *));

#endif

