#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <binn.h>
#include <string.h>

#include "lib/linked_list.h"
#include "lib/transaction.h"
#include "lib/block.h"
#include "lib/blockchain.h"
#include "test/test_utils.h"

void dummy_free(void *unused) {}

void int_to_string(void *value, char *buf) {
  int *int_value = (int *)value;
  sprintf(buf, "%i", *int_value);
}

void test_ll_add_delete(int *passing, int *total) {
  linked_list *ll = ll_init();

  int a = 21;
  int b = 42;
  int c = 53;

  track_test(test_bool(ll->head == NULL && ll->last == NULL,
                       "Head and last are NULL on init"),
             passing, total);

  ll_append(ll, &a);
  ll_append(ll, &b);
  ll_append(ll, &c);

  track_test(test_bool(&a == ll_get(ll, 0), "First node is appended"), passing,
             total);
  track_test(test_bool(&b == ll_get(ll, 1), "Second node is appended"), passing,
             total);
  track_test(test_bool(&c == ll_get(ll, 2), "Third node is appended"), passing,
             total);

  track_test(test_bool(3 == ll->size, "Size increases on append"), passing,
             total);

  ll_delete(ll, 1, dummy_free);

  track_test(test_bool(!ll_contains(ll, &b), "Second node is removed"), passing,
             total);

  ll_delete(ll, 1, dummy_free);

  track_test(test_bool(!ll_contains(ll, &b), "Third node is removed"), passing,
             total);

  ll_delete(ll, 0, dummy_free);

  track_test(test_bool(!ll_contains(ll, &b), "First node is removed"), passing,
             total);

  track_test(test_bool(ll->head == NULL && ll->last == NULL,
                       "Head and last are NULL when list is empty"),
             passing, total);

  track_test(test_bool(0 == ll->size, "Size reduces on remove"), passing,
             total);

  ll_free(ll, dummy_free);
}

void test_linked_list_functions(int *passing, int *total) {
  printf("---------------------------------------------------------------------"
         "\n");
  printf("-----%sLINKED LIST "
         "TESTS%s-----------------------------------------------\n",
         BOLDBLUE, NOCOLOUR);
  int internal_passing = 0;
  int internal_total = 0;

  test_ll_add_delete(&internal_passing, &internal_total);

  printf("---------------------------------------------------------------------"
         "\n");
  printf("%sPASSING: (%d/%d) tests%s\n",
         internal_passing == internal_total        ? GREEN
         : (internal_passing > internal_total / 2) ? YELLOW
                                                   : RED,
         internal_passing, internal_total, NOCOLOUR);

  *passing = *passing + internal_passing;
  *total = *total + internal_total;
}

void test_serialize_transaction(int *passing, int *total) {

  // Setup transaction
  transaction *t = init_transaction("ash", "ash21", 1200, -22118400);
  binn *serialized = serialize_transaction(t);
  transaction *other_end = deserialize_transaction(serialized);
  binn_free(serialized);

  track_test(
      test_transaction(t, other_end, "Transactions are equal on both ends"),
      passing, total);

  /*
  printf("before serializing  : ");
  print_transaction(t);
  printf("after deserializing : ");
  print_transaction(other_end);
  */

  // Free transactions
  free_transaction(t);
  free_transaction(other_end);

}

void test_serialize_transaction_list(int *passing, int *total) {

  // Create transactions
  transaction *t1 = init_transaction("blockchain_overlord", "ash", 1000,
		-22118400);
  transaction *t2 = init_transaction("blockchain_overlord", "sid", 1000,
		-22118400);
  transaction *t3 = init_transaction("blockchain_overlord", "kavya", 1000,
		-22118400);
  transaction *t4 = init_transaction("blockchain_overlord", "yelun", 1000,
		-22118400);

  // Create list and add transactions
  linked_list *transactions = ll_init();
  ll_append(transactions, t1);
  ll_append(transactions, t2);
  ll_append(transactions, t3);
  ll_append(transactions, t4);

  binn *serialized = serialize_transactions(transactions);
  linked_list *other_end =
      deserialize_transactions(serialized);
  binn_free(serialized);

  track_test(
      test_linked_list(transactions, other_end, to_string_transaction,
                       "Linked list of transactions is equal on both ends"),
      passing, total);

  /*
  printf("before serializing  : \n");
  ll_print(transactions, to_string_transaction);
  printf("after deserializing : \n");
  ll_print(other_end, to_string_transaction);
  */

  // Free linked list and transactions
  ll_free(transactions, free_transaction);
  ll_free(other_end, free_transaction);
}

void test_serialize_block_no_hash(int *passing, int *total) {

  block *genesis = GENESIS_BLOCK;

  // we want to test serialize without hashing
  memset(genesis->hash, 0, sizeof(genesis->hash));

  // we also want to check if prev hash gets serialised
  strcpy(genesis->prev_hash, "It's previous hash lmao");

  binn *serialized = serialize_block_no_hash(genesis);
  block *other_end = deserialize_block(serialized);
  binn_free(serialized);

  track_test(
      test_block(genesis, other_end, "Genesis block is equal on both ends"),
      passing, total);


  /*
  printf("before serializing  : \n");
  print_block(genesis);
  printf("after deserializing : \n");
  print_block(other_end);
  */

  free_block(genesis);
  free_block(other_end);
}

void test_serialize_deserialize(int *passing, int *total) {
  printf("---------------------------------------------------------------------"
         "\n");
  printf("-----%sSERIALIZE/DESERIALIZE "
         "TESTS%s-------------------------------------\n",
         BOLDBLUE, NOCOLOUR);
  int internal_passing = 0;
  int internal_total = 0;

  test_serialize_transaction(&internal_passing, &internal_total);
  test_serialize_transaction_list(&internal_passing, &internal_total);
  test_serialize_block_no_hash(&internal_passing, &internal_total);

  printf("-----------------------------------------------------------------"
         "----"
         "\n");
  printf("%sPASSING: (%d/%d) tests%s\n",
         internal_passing == internal_total        ? GREEN
         : (internal_passing > internal_total / 2) ? YELLOW
                                                   : RED,
         internal_passing, internal_total, NOCOLOUR);

  *passing = *passing + internal_passing;
  *total = *total + internal_total;

}

void test_hash_equality(int *passing, int *total) {
  block *genesis = GENESIS_BLOCK;

  hash *first_hash = hash_block(genesis);
  hash *second_hash = hash_block(genesis);

  track_test(test_hash(first_hash, second_hash, "Both hashes are equal"),
             passing, total);

  free_block(genesis);
  free(first_hash);
  free(second_hash);
}

void test_proof_of_work_function(int *passing, int *total) {
  blockchain *bc = init_blockchain();
  char *username = "rick";
  block *just_mined = proof_of_work(bc, username);
  track_test(test_bool(is_valid(just_mined), "Latest block is valid"),
             passing, total);
  track_test(
      test_bool(just_mined != bc->latest_block, "Latest block is actually new"),
      passing, total);

  // Print new block
  printf("Just mined: ");
  print_block(just_mined);

  free_block(just_mined);
  free_blockchain(bc);
}

void test_proof_of_work(int *passing, int *total) {
  printf("---------------------------------------------------------------------"
				"\n");
  printf("-----%s HASH/PROOF OF WORK"
         "TESTS%s-------------------------------------\n",
         BOLDBLUE, NOCOLOUR);
	int internal_passing = 0;
	int internal_total = 0;

	test_hash_equality(&internal_passing, &internal_total);
  test_proof_of_work_function(&internal_passing, &internal_total);

	printf("-----------------------------------------------------------------"
				"----"
				"\n");
  printf("%sPASSING: (%d/%d) tests%s\n",
         internal_passing == internal_total        ? GREEN
         : (internal_passing > internal_total / 2) ? YELLOW
                                                   : RED,
         internal_passing, internal_total, NOCOLOUR);

  *passing = *passing + internal_passing;
  *total = *total + internal_total;
}

void test_append_blocks(int *passing, int *total) {
  blockchain *first_bc = init_blockchain();
  blockchain *second_bc = init_blockchain();
  track_test(
    test_blockchain(
      first_bc,
      second_bc,
      "Blockchain initialization produces identical results."
    ),
    passing, total);
  // Print new block
  printf("Blockchain 1: \n");
  print_blockchain(first_bc);
  printf("Blockchain 2: \n");
  print_blockchain(second_bc);

  // Add 1 new node to both blockchains

  char *username = "rick";
  block *just_mined = proof_of_work(first_bc, username);
  block *just_mined_dup = calloc(1, sizeof(block));
  memcpy(just_mined_dup, just_mined, sizeof(block));
  just_mined_dup->prev_block = second_bc->latest_block;
  just_mined_dup->reward = *dup_transaction(&just_mined->reward);
  append_to_blockchain(first_bc, just_mined);
  append_to_blockchain(second_bc, just_mined_dup);
  track_test(
    test_blockchain(
      first_bc,
      second_bc,
      "Adding the same block to both chains produces identical results."
    ),
    passing, total);
  printf("Blockchain 1: \n");
  print_blockchain(first_bc);
  printf("Blockchain 2: \n");
  print_blockchain(second_bc);

  block *next_mined = proof_of_work(first_bc, username);
  block *next_mined_dup = calloc(1, sizeof(block));
  memcpy(next_mined_dup, next_mined, sizeof(block));
  next_mined_dup->prev_block = second_bc->latest_block;
  next_mined_dup->reward = *dup_transaction(&next_mined->reward);
  append_to_blockchain(first_bc, next_mined);
  append_to_blockchain(second_bc, next_mined_dup);
  track_test(
    test_blockchain(
      first_bc,
      second_bc,
      "Adding the same block to both chains produces identical results."
    ),
    passing, total);
  printf("Blockchain 1: \n");
  print_blockchain(first_bc);
  printf("Blockchain 2: \n");
  print_blockchain(second_bc);

  free(just_mined);
  free(just_mined_dup);
  free_blockchain(first_bc);
  free_blockchain(second_bc);
  free(next_mined);
  free(next_mined_dup);
}

void test_blockchain_append(int *passing, int *total) {
  printf("---------------------------------------------------------------------"
				"\n");
  printf("-----%s BLOCKCHAIN APPENDING "
         "TESTS%s-------------------------------------\n",
         BOLDBLUE, NOCOLOUR);

	int internal_passing = 0;
	int internal_total = 0;

  test_append_blocks(&internal_passing, &internal_total);

	printf("-----------------------------------------------------------------"
				"----"
				"\n");
  printf("%sPASSING: (%d/%d) tests%s\n",
         internal_passing == internal_total        ? GREEN
         : (internal_passing > internal_total / 2) ? YELLOW
                                                   : RED,
         internal_passing, internal_total, NOCOLOUR);

  *passing = *passing + internal_passing;
  *total = *total + internal_total;
}

int main(void) {
  int passing = 0;
  int total = 0;

  test_linked_list_functions(&passing, &total);
  test_serialize_deserialize(&passing, &total);
	// test_proof_of_work(&passing, &total);
  test_blockchain_append(&passing, &total);

  printf(
      "%s---------------------------------------------------------------------%"
      "s\n",
      BOLDWHITE, NOCOLOUR);
  printf("%sTOTAL PASSING: (%d/%d) tests%s\n",
         passing == total        ? BOLDGREEN
         : (passing > total / 2) ? BOLDYELLOW
                                 : BOLDRED,
         passing, total, NOCOLOUR);
  printf(
      "%s---------------------------------------------------------------------%"
      "s\n",
      BOLDWHITE, NOCOLOUR);
  return EXIT_SUCCESS;
}
