#include "linked_list.h"
#include <assert.h>
#include <stdint.h>
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

void append_to_linked_list(linked_list *list, void *val) {
  assert(list);
  assert(val);

  list->size +=1;

  if (list->head == NULL) {
    list->head = init_node(val);
    return;
  }

  node *curr = list->head;
  while(curr->next != NULL){
    curr = curr->next;
  }
  curr->next = init_node(val);
}

node *traverse_linked_list(linked_list *list) {
  assert(list);

  node *curr = list->head;

  while (curr->next != NULL){
    curr = curr->next;
  }
  return curr;
}

void free_node(node *to_free, void (*value_free)(void *)){
  value_free(to_free->value);
  free(to_free->value);
  free(to_free);
}

void delete_node(node *to_delete, linked_list *list, void (*value_free)(void *)) {
  assert(list);
  assert(to_delete);

  node *prev_node = list->head;
  while (prev_node->next != to_delete){
    prev_node = prev_node->next;
  }
  prev_node->next = to_delete->next;
  free_node(to_delete, value_free);
}
  

void free_linked_list(linked_list *list, void (*value_free)(void *)) {
  assert(list);

  node *curr = list->head;
  while (curr) {
    node *temp = curr;
    curr = curr->next;
    free_node(temp, value_free);
  }
  free(list);
}
