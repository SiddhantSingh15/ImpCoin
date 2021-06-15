#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <binn.h>
#include <time.h>

#include "linked_list.h"
#include "transaction.h"

transaction *init_transaction(char *from, char *to, uint64_t amount) {
  transaction *new = calloc(1, sizeof(transaction));
  new->timestamp = time(NULL);
  new->amount = amount;
  strncpy(new->from, from, UID_LENGTH);
  strncpy(new->to, to, UID_LENGTH);
  return new;
}

binn *serialize_transaction(transaction *single) {
  binn *obj = binn_object();

  binn_object_set_uint64(obj, "timestamp", single->timestamp);
  binn_object_set_str(obj, "to", &single->to[0]);
  binn_object_set_str(obj, "from", &single->from[0]);
  binn_object_set_uint64(obj, "amount", single->amount);

  return obj;
}

binn *serialize_transactions(linked_list *ts) {
  binn *list = binn_list();

  ll_node *curr = ts->head;
  while (curr != NULL) {
    binn *obj = serialize_transaction(curr->value);
    binn_list_add_object(list, obj);
    binn_free(obj);
    curr = curr->next;
  }

  return list;
}

transaction *deserialize_transaction(binn *trn) {
  transaction *trans = malloc(sizeof(transaction));

  trans->timestamp = binn_object_uint64(trn, "timestamp");
  strcpy(trans->to, binn_object_str(trn, "to"));
  strcpy(trans->from, binn_object_str(trn, "from"));
  trans->amount = binn_object_uint64(trn, "amount");

  return trans;
}

linked_list *deserialize_transactions(binn *transactions) {

  linked_list *new_ll = ll_init();

  binn_iter iter;
  binn value;
  binn_list_foreach(transactions, value) {
    ll_append(new_ll, deserialize_transaction(&value));
  }

  return new_ll;
}

void to_string_transaction(void *t, char *buffer) {
  transaction *tr = (transaction *)t;
  char fmttime[100];
  strftime(fmttime, 100, "%m/%d/%y %X UTC", gmtime(&tr->timestamp));
  sprintf(buffer, "[%"PRIu64" Coin] %s -> %s @ %s", tr->amount, tr->from, tr->to,
          fmttime);
}

void print_transaction(transaction *t) {
  char buffer[511];
  to_string_transaction(t, buffer);
  printf("%s\n", buffer);
}

void free_transaction(void *t) {
  free(t);
}
