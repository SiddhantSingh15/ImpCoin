#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>

/**
 * @brief The design of a node that can be stored in the linked list. It stores
 * a line in the given file as an instr_t type or as tokens, and also stores the
 * address of that line.
 */
typedef struct node {
  void *value;
  uint32_t address;
  struct node *next;
} node;

/**
 * @brief The linked list of nodes that will contain lines of the file as
 * instr_t or as tokens.
 */
typedef struct linked_list {
  struct node *head;
} linked_list;

/**
 * @brief Initialises a new linked list with a null head node.
 * 
 * @return The initialised linked list.
 */
linked_list *init_linked_list(void);

/**
 * @brief Initialises a new linked list node with a specified address and val,
 * pointing to a next node of NULL.
 *
 * @param address The address of the initialised node.
 * @param val The value to be stored in the node.
 *
 * @return The initialized node.
 */
node *init_node(uint32_t address, void *val);

/**
 * @brief Appends a new node with given value and address to the list. The list is
 * accessed via the given entry node.
 *
 * @param entry The node through which we access the list.
 * @param val The value to node to be appended.
 *
 * @return The memory address of the appended node, i.e. its position in the
 * linked list time 4.
 */
uint32_t append_via_node(node *entry, void *val);

/**
 * @brief Adds a new node to the linked list. Generally used when the file is
 * first read and the lines of the file must be seperated into nodes as strings.
 *
 * @param list The list to which the node must be added.
 * @param val The value of the node to be appended.
 *
 * @return The memory address of the appended node, i.e. its position in the
 * linked list time 4.
 */
uint32_t append_to_linked_list(linked_list *list, void *val);

/**
 * @brief Traverses the given linked list and returns the node at a specified
 * address.
 *
 * @param list The linked list to be traversed.
 * @param address The address of the node that must be returned.
 *
 * @return A pointer to node at specified address.
 */
node *traverse_linked_list(linked_list *list, uint32_t address);

/**
 * @brief Changes the value of the node in the given linked list at a specified
 * address.
 *
 * @param list The linked list to where the node to be changed is present.
 * @param pos The address of the node to be changed.
 * @param val The new value to be assigned to the node.
 */
void change_node(linked_list *list, uint32_t address, void *val);

/**
 * @brief Frees the allocated memory of given list, all nodes contained in it
 * and the components associated to each node.
 *
 * @param list The list to be freed.
 */
void free_linked_list(linked_list *list);

#endif
