#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <binn.h>
#include <time.h>
#include <pthread.h>

#include "linked_list.h"
#include "transaction.h"
#include "block.h"
#include "blockchain.h"
#include "messages.h"
#include "utils.h"

binn *serialize_bc_msg(blockchain_msg *bc_msg) {
  binn *obj = binn_object();
  binn *bc = serialize_blockchain(bc_msg->bc);
  binn_object_set_object(obj, "blockchain", bc);
  binn_free(bc);
  binn_object_set_str(obj, "username", bc_msg->username);
  binn_object_set_str(obj, "type", bc_msg->type);

  return obj;
}

blockchain_msg *deserialize_bc_msg(binn *input) {
  blockchain_msg *bc_msg = calloc(1, sizeof(blockchain_msg));
  bc_msg->bc = deserialize_blockchain(binn_object_object(input, "blockchain"));
  strncpy(bc_msg->username, binn_object_str(input, "username"), 40);
  strncpy(bc_msg->type, binn_object_str(input, "type"), 10);
  return bc_msg;
}

binn *serialize_t_msg(transaction_msg *t_msg) {
  binn *obj = binn_object();
  binn_object_set_uint16(obj, "amount", t_msg->amount);
  binn_object_set_str(obj, "username", t_msg->username);
  binn_object_set_str(obj, "type", t_msg->type);
  binn_object_set_str(obj, "to", t_msg->to);

  return obj;
}

transaction_msg *deserialize_t_msg(binn *input) {
  transaction_msg *t_msg = calloc(1, sizeof(transaction_msg));
  strncpy(t_msg->username, binn_object_str(input, "username"), 40);
  strncpy(t_msg->type, binn_object_str(input, "type"), 10);
  strncpy(t_msg->to, binn_object_str(input, "to"), 10);
  t_msg->amount = binn_object_uint64(input, "amount");

  return t_msg;
}