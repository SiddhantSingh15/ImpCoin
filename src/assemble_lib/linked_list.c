#include "linked_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

void append(struct linked_list *list, void *val, uint32_t addr) {
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

  struct node *curr = list->root;

  while (curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = new_node;
  list->size += 1;
}

node *traverse(struct linked_list *list, int pos) {
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

  node *node_to_change = traverse(list, pos);
  node_to_change->value = val;
}
