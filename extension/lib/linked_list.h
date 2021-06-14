#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>

typedef struct ll_node {
  void *value;
  struct ll_node *next;
} ll_node;

typedef struct linked_list {
  ll_node *head;
  uint32_t size;
} linked_list;

linked_list *init_linked_list(void);

ll_node *init_node(void *val);

void append_to_linked_list(linked_list *list, void *val);

void free_node(ll_node *to_free, void (*value_free)(void *));

void delete_node(ll_node *to_delete, linked_list *list,
                 void (*value_free)(void *));

void print_linked_list(linked_list *list, void (*value_print)(void *));

void free_linked_list(linked_list *list, void (*value_free)(void *));

#endif
