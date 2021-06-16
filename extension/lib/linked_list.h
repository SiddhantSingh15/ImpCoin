#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include <stdbool.h>

typedef struct ll_node {
  void *value;
  struct ll_node *next;
} ll_node;

typedef struct linked_list {
  ll_node *head;
  ll_node *last;
  uint32_t size;
} linked_list;

linked_list *ll_init(void);

ll_node *ll_init_node(void *val);

void ll_append(linked_list *list, void *val);

void ll_clear(linked_list *list);

ll_node *ll_get_node(linked_list *list, uint32_t index);

void *ll_get(linked_list *list, uint32_t index);

bool ll_contains(linked_list *list, void *val);

void ll_free_node(ll_node *to_free, void (*value_free)(void *));

void ll_delete(linked_list *list, uint32_t index, void (*value_free)(void *));

void ll_delete_node(linked_list *list, ll_node *to_delete,
                    void (*value_free)(void *));

void ll_drop(linked_list *list, uint32_t number, void (*value_free)(void *));

char *ll_to_string(linked_list *list,
                  void (*value_to_string)(void *, char *buf));

void ll_print(linked_list *list, void (*value_to_string)(void *, char *buf));

void ll_free(linked_list *list, void (*value_free)(void *));

#endif
