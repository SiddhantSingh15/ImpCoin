#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>

struct node {
  void *value;
  uint32_t address;
  struct node *next;
} node;

struct linked_list {
  struct node *root;
  int size;
} linked_list;

void append(struct linked_list *list, void *val, uint32_t addr);

struct node traverse(struct linked_list *list, int pos);

void change_node(struct linked_list *list, int pos, void *val);

#endif
