#include "block.h"
#include "transaction.h"
#include <binn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

binn *serialize_transaction(transaction *single) {
  binn *trans = binn_object();

  binn_object_set_uint64(trans, "timestamp", single->timestamp);
  binn_object_set_str(trans, "to", &single->to[0]);
  binn_object_set_str(trans, "from", &single->from[0]);
  binn_object_set_uint64(trans, "amount", single->amount);

  return trans;
}

binn *serialize_transactions(transaction *transactions) {
  binn *list = binn_list();

  for (int i  = 0; i < sizeof(transactions); i++) {
    binn *obj = serialize_transaction(&transactions[i]);
    binn_list_add_object(list, obj);
  }

  return list; 
}

transaction *deserialize_transaction(binn *trn) {
  transaction *trans;

  trans->timestamp = binn_object_uint64(trn, "timestamp");
  strcpy(trans->to, binn_object_str(trn, "to"));
  strcpy(trans->from, binn_object_str(trn, "from"));
  trans->amount = binn_object_uint64(trn, "amount");

  return trans;
}

transaction *deserialize_transactions(binn *transactions) {
  
  transaction *trns;
  for (int i = 0; i < MAX_TRANSACTIONS_PER_BLOCK; i++) {
    trns[i] = *deserialize_transaction(binn_list_object(transactions, i));
  }
  
  return trns;
}