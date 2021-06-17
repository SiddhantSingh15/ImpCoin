#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <binn.h>
#include <time.h>

#include "utils.h"
#include "linked_list.h"
#include "transaction.h"
#include "block.h"
#include "blockchain.h"

transaction *init_transaction(char *from, char *to, uint64_t amount,
  time_t time) {
  transaction *new = calloc(1, sizeof(transaction));
  new->timestamp = time;
  new->amount = amount;
  strncpy(new->from, from, UID_LENGTH);
  strncpy(new->to, to, UID_LENGTH);
  return new;
}

transaction *dup_transaction(transaction *t) {
  transaction *dup = init_transaction(t->from, t->to, t->amount, t->timestamp);
  return dup;
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

  if (ts == NULL) {
    return list;
  }

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

bool is_valid_transaction(transaction *tc, void *bc_ptr) {
  blockchain *bc = bc_ptr;
  uint64_t amount = 0;
  block *curr_b = bc->latest_block;
  linked_list *mempool = bc->mempool;
  ll_node *curr_lln;
  // Check blockchain
  while (curr_b != NULL) {
    curr_lln = (curr_b->transactions) ? curr_b->transactions->head : NULL;
    while (curr_lln != NULL) {
      transaction *trans = (transaction *) curr_lln->value;
      if (strcmp(trans->to, tc->from) == 0) {
        amount += trans->amount;
      } else if (strcmp(trans->from, tc->from) == 0) {
        amount -= trans->amount;
      }
      curr_lln = curr_lln->next;
    }
    amount += strcmp(curr_b->reward.to, tc->from) == 0 ?
      curr_b->reward.amount : 0;
    curr_b = curr_b->prev_block;
  }
  // Check current mempool
  curr_lln = mempool->head;
  while (curr_lln != NULL) {
    transaction *trans = (transaction *) curr_lln->value;
    printf("%s sent %ld to %s\n", trans->from, trans->amount, trans->to);
    if (strcmp(trans->to, tc->from) == 0) {
        amount += trans->amount;
      } else if (strcmp(trans->from, tc->from) == 0) {
        amount -= trans->amount;
      }
    curr_lln = curr_lln->next;
  }

  return tc->amount < amount;
}

void to_string_transaction(void *t, char *buffer) {
  transaction *tr = (transaction *)t;
  char *fmtedtime = formatted_time(&tr->timestamp);
  sprintf(buffer, "[%"PRIu64" IMP%s] %s -> %s @ %s", tr->amount,
          (tr->amount > 1) ? "s" : "", tr->from, tr->to, fmtedtime);
  free(fmtedtime);
}

void print_transaction(transaction *t) {
  char buffer[511];
  to_string_transaction(t, buffer);
  printf("%s\n", buffer);
}

void free_transaction(void *t) {
  free(t);
}
