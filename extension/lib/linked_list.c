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

void append_via_node(node *entry, void *val) {
  assert(entry != NULL);
  assert(val);

  //add 1 to size of list?

  node *curr = entry;
  while(curr->next != NULL){
    curr = curr->next;
  }

  curr->next = init_node(val);
}

void append_to_linked_list(linked_list *list, void *val) {
  if (list->head == NULL) {
    list->head = init_node(val);
    list->size = 1;
    return;
  }

  list->size += 1;
  append_via_node(list->head, val);
}

node *traverse_linked_list(linked_list *list) {
  assert(list);

  node *curr = list->head;

  while (curr->next != NULL){
    curr = curr->next;
  }
  return curr;
}

void delete_node(node *to_delete, linked_list *list) {}
  

void free_linked_list(linked_list *list, void (*value_free)(void *)) {
  assert(list);

  node *curr = list->head;
  while (curr) {
    node *temp = curr;
    curr = curr->next;
    //value_free(tem->value);
    free(temp->value);
    free(temp);
  }
  free(list);
}
