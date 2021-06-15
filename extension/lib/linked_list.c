#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "linked_list.h"

linked_list *ll_init(void) {
  linked_list *new_list = malloc(sizeof(linked_list));
  new_list->head = NULL;
  new_list->last = NULL;
  new_list->size = 0;
  return new_list;
}

ll_node *ll_init_node(void *val) {
  ll_node *new_node = malloc(sizeof(ll_node));
  new_node->value = val;
  new_node->next = NULL;
  return new_node;
}

void ll_append(linked_list *list, void *val) {
  assert(list);
  assert(val);

  list->size +=1;

  if (list->head == NULL) {
    assert(list->last == NULL);
    list->head = ll_init_node(val);
    list->last = list->head;
    return;
  }

  list->last->next = ll_init_node(val);
  list->last = list->last->next;
}

ll_node *ll_get_node(linked_list *list, uint32_t index) {
  assert(list);

  int i = 0;
  ll_node *curr = list->head;
  while (curr != NULL && i < index){
    curr = curr->next;
    i++;
  }

  return curr;
}

void *ll_get(linked_list *list, uint32_t index) {
  assert(list);
  ll_node *node = ll_get_node(list, index);
  return (node != NULL) ? node->value : NULL;
}

// Checks if a given pointer is contained inside a linked list, as a value.
bool ll_contains(linked_list *list, void *val) {
  assert(list);
  assert(val);

  ll_node *curr = list->head;
  while (curr != NULL) {
    if (curr->value == val) {
      return true;
    }
    curr = curr->next;
  }

  return false;
}

void ll_free_node(ll_node *to_free, void (*value_free)(void *)){
  value_free(to_free->value);
  free(to_free);
}

void ll_delete_node(linked_list *list, ll_node *to_delete,
                 void (*value_free)(void *)) {
  assert(list);
  assert(list->size > 0);
  assert(to_delete);

  list->size--;

  if (list->head == to_delete && list->last == to_delete) {
    // If there is only one node
    list->head = NULL;
    list->last = NULL;
    ll_free_node(to_delete, value_free);
    return;

  } else if (list->head == to_delete) {
    // If the head is removed
    list->head = list->head->next;
    ll_free_node(to_delete, value_free);
    return;
  }

  ll_node *prev = list->head;
  while (prev->next != to_delete && prev->next != NULL) {
    prev = prev->next;
  }

  // If the last is removed
  if (prev->next == list->last) {
    list->last = prev;
  }

  prev->next = to_delete->next;
  ll_free_node(to_delete, value_free);
}

void ll_delete(linked_list *list, uint32_t index, void (*value_free)(void *)) {
  ll_node *node = ll_get_node(list, index);
  ll_delete_node(list, node, value_free);
}

void ll_free(linked_list *list, void (*value_free)(void *)) {
  assert(list);

  ll_node *curr = list->head;
  while (curr) {
    ll_node *temp = curr;
    curr = curr->next;
    ll_free_node(temp, value_free);
  }
  free(list);
}
