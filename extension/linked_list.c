#include "linked_list.h"
#include <stdlib.h>

linked_list *init_linked_list(void) {
  linked_list *new_list = malloc(sizeof(linked_list));
  new_list->head = NULL;
  new_list->size = 0;
  return new_list;
}

node *init_node(void *val) {
  node *new_node = malloc(sizeof(node));
  new_node->value = val;
  new_node->next = NULL;
  return new_node;
}
