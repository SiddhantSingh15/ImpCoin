#include "linked_list.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

linked_list *init_linked_list(void) {
  linked_list *new_list = malloc(sizeof(linked_list));
  new_list->head = NULL;
  new_list->last = NULL;
  new_list->size = 0;
  return new_list;
}

ll_node *init_node(void *val) {
  ll_node *new_node = malloc(sizeof(ll_node));
  new_node->value = val;
  new_node->next = NULL;
  return new_node;
}

void append_to_linked_list(linked_list *list, void *val) {
  assert(list);
  assert(val);

  list->size +=1;

  if (list->head == NULL) {
    assert(list->last == NULL);
    list->head = init_node(val);
    list->last = list->head;
    return;
  }

  list->last->next = init_node(val);
  list->last = list->last->next;
}

ll_node *traverse_linked_list(linked_list *list) {
  assert(list);

  ll_node *curr = list->head;

  while (curr->next != NULL){
    curr = curr->next;
  }
  return curr;
}

void free_node(ll_node *to_free, void (*value_free)(void *)){
  value_free(to_free->value);
  free(to_free->value);
  free(to_free);
}

void delete_node(ll_node *to_delete, linked_list *list,
                 void (*value_free)(void *)) {
  assert(list);
  assert(list->size > 0);
  assert(to_delete);

  list->size--;

  if (list->head == to_delete && list->last == to_delete) {
    // If there is only one node
    list->head = NULL;
    list->last = NULL;
    free_node(to_delete, value_free);
    return;

  } else if (list->head == to_delete) {
    // If the head is removed
    list->head = list->head->next;
    free_node(to_delete, value_free);
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
  free_node(to_delete, value_free);
}

void free_linked_list(linked_list *list, void (*value_free)(void *)) {
  assert(list);

  ll_node *curr = list->head;
  while (curr) {
    ll_node *temp = curr;
    curr = curr->next;
    free_node(temp, value_free);
  }
  free(list);
}
