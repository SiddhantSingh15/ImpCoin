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

#endif

