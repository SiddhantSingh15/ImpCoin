#include "linked_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

linked_list *init_linked_list(void) {
  linked_list *list = malloc(sizeof(linked_list));
  list->root = NULL;
  list->size = 0;
  return list;
}

void append_to_linked_list(linked_list *list, void *val, uint32_t addr) {
  assert(val);

  node *new_node = malloc(sizeof(node));
  new_node->value = val;
  new_node->address = addr;
  new_node->next = NULL;

  if (list->root == NULL) {
    list->size = 1;
    list->root = new_node;
    return;
  }

  node *curr = list->root;

  while (curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = new_node;
  list->size += 1;
}

node *traverse_linked_list(linked_list *list, int pos) {
  assert(list);
  assert(pos);
  assert((0 <= pos) < list->size);

  struct node *curr;
  curr = list->root;

  while (pos >= 0) {
    pos--;
    curr = curr->next;
  }

  return curr;
}

void change_node(linked_list *list, int pos, void *val) {
  assert(list);
  assert((0 <= pos) < list->size);

  node *node_to_change = traverse_linked_list(list, pos);
  node_to_change->value = val;
}
