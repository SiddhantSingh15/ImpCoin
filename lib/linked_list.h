#ifndef linked_list_h
#define linked_list_h

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief a struct for the node type that is to be contained within the linked
 *  list. it contains a pointer to a value of any (void) type and a pointer to
 *  the next node in the list.
 */
typedef struct ll_node {
  void *value;
  struct ll_node *next;
} ll_node;

/**
 * @brief a struct for the linked list that contains the nodes as specified 
 *  above. it contains the first (head) node of the list, the final node in 
 *  the list and the size of the list (i.e the number of nodes contained in
 *  the list).
 */
typedef struct linked_list {
  ll_node *head;
  ll_node *last;
  uint32_t size;
} linked_list;

/**
 * @brief creates (using calloc) a new linked list, initializing it's head, 
 *  final node to null and size to 0.
 *
 * @return the initialized linked list.
 */
linked_list *ll_init(void);

/**
 * @brief creates (using malloc) a new node, setting its value to the value 
 *  passed into the method.
 *
 *  @param val the value to be stored in the node.
 *
 *  @return the new initialized node.
 */
ll_node *ll_init_node(void *val);

/**
 * @brief initializes a new node with the given value and appends the node to
 *  the given linked list.
 *
 * @param list the linked list to which the node must be appended.
 * @param val the value of the new node to be appended.
 */
void ll_append(linked_list *list, void *val);

/**
 * @brief resets the head and last nodes of the linked list to null and size
 *  to 0.
 *
 * @param list the linked list to be cleared
 */
void ll_clear(linked_list *list);

/**
 * @brief returns the node in a linked list at a specified index.
 *
 * @param list the list to be searched for the node.
 * @param index the index of the node to be returned.
 *
 * @return the node at the specified index.
 */
ll_node *ll_get_node(linked_list *list, uint32_t index);

/**
 * @brief returns the node at a given index in the linked list if the index is
 *  within the size range of the linked list and if the node returned by 
 *  ll_get_node is not null.
 *
 * @param list the list to be searched for the node.
 * @param index the index of the node to be returned.
 *
 * @return null if the node at the index is null (index is out of bounds of 
 *  the linked list), or the value of the node if the node exists and is not
 *  null.
 */
void *ll_get(linked_list *list, uint32_t index);

/**
 * @brief checks if a given pointer to a value is contained within any of the
 *  nodes of the given linked list.
 *
 * @param list the linked list to be searched.
 * @param val the pointer to the value that is being searched for.
 *
 * @return true if the pointer to the value is contained in the linked list and
 *  false otherwise.
 */
bool ll_contains(linked_list *list, void *val);

/**
 * @brief frees the memory space allocated to a particular node.
 *
 * @param to_free a pointer to the node that is to be freed.
 * @param value_free a pointer to the function that is used to free the value
 *  contained within the node.
 */
void ll_free_node(ll_node *to_free, void (*value_free)(void *));

/**
 * @brief frees the memory allocated to and deletes the node at a specified
 *  index in the given linked list.
 *
 * @param list the list from which the node is to be deleted.
 * @param index the index of the node to be deleted.
 */
void ll_delete(linked_list *list, uint32_t index, void (*value_free)(void *));

/**
 * @brief deletes the given node from the given linked list and frees the space
 *  allocated to that node.
 *
 * @param list the list from which the node is to be deleted.
 * @param to_delete a pointer to the node that must be deleted from the list.
 * @param value_free a pointer to the function that is used to free the value
 *  assigned in the node.
 */
void ll_delete_node(linked_list *list, ll_node *to_delete,
                    void (*value_free)(void *));

/**
 * @brief clears/removes the nodes starting from the head of the linked list 
 *  up until a specified index, by freeing them.
 *
 * @param list the linked list from which the nodes must be deleted.
 * @param number the index up until which nodes must be deleted.
 * @param value_free a pointer to the function used to free the values 
 *  contained within the nodes of the linked list.
 */
void ll_drop(linked_list *list, uint32_t number, void (*value_free)(void *));

/**
 * @brief converts a given list to a string that contains the values contained
 *  within all the nodes of the list along with the indices of the nodes.
 *
 * @param list the linked list to be printed.
 * @param value_to_string a pointer to the function used to print the values
 *  contained within the strings.
 *
 * @return the final string containing all indices and values of the nodes.
 */
char *ll_to_string(linked_list *list,
                  void (*value_to_string)(void *, char *buf));

/**
 * @brief prints the given linked list afer converting it to string using the
 *  ll_to_string function.
 *
 * @param list the list to be printed.
 * @param value_to_string a pointer to the function used to convert the values
 *  of the nodes to string.
 */
void ll_print(linked_list *list, void (*value_to_string)(void *, char *buf));

/**
 * @brief frees the memory spaces allocated to the given linked list along 
 *  with all the nodes and values contained in it.
 *
 * @param list the list to be freed.
 * @param value_free a pointer to the function used to free the values 
 *  contained within the nodes.
 */
void ll_free(linked_list *list, void (*value_free)(void *));

#endif
