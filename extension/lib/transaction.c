#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <binn.h>

#include "linked_list.h"
#include "transaction.h"

binn *serialize_transaction(transaction *single) {
  binn *trans = binn_object();

  binn_object_set_uint64(trans, "timestamp", single->timestamp);
  binn_object_set_str(trans, "to", &single->to[0]);
  binn_object_set_str(trans, "from", &single->from[0]);
  binn_object_set_uint64(trans, "amount", single->amount);

  return trans;
}

binn *serialize_transactions(linked_list *transactions) {
  binn *list = binn_list();

  ll_node *curr = transactions->head;
  while (curr != NULL) {
    binn *obj = serialize_transaction(curr->value);
    binn_list_add_object(list, obj);
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
