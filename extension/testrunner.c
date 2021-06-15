#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <binn.h>

#include "lib/linked_list.h"
#include "lib/transaction.h"
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

  transaction *t = init_transaction("ash", "ash21", 1200);
  transaction *other_end = deserialize_transaction(serialize_transaction(t));

  track_test(
      test_transaction(t, other_end, "Transactions are equal on both ends"),
      passing, total);

  printf("before serializing  : ");
  print_transaction(t);
  printf("after deserializing : ");
  print_transaction(other_end);

}

void test_serialize_transaction_list(int *passing, int *total) {

  // Create transactions
  transaction *t1 = init_transaction("blockchain_overlord", "ash", 1000);
  transaction *t2 = init_transaction("blockchain_overlord", "sid", 1000);
  transaction *t3 = init_transaction("blockchain_overlord", "kavya", 1000);
  transaction *t4 = init_transaction("blockchain_overlord", "yelun", 1000);

  // Create list and add transactions
  linked_list *transactions = ll_init();
  ll_append(transactions, t1);
  ll_append(transactions, t2);
  ll_append(transactions, t3);
  ll_append(transactions, t4);

  linked_list *other_end =
      deserialize_transactions(serialize_transactions(transactions));

  track_test(
      test_linked_list(transactions, other_end, to_string_transaction,
                       "Linked list of transactions is equal on both ends"),
      passing, total);

  printf("before serializing  : \n");
  ll_print(transactions, to_string_transaction);
  printf("after deserializing : \n");
  ll_print(other_end, to_string_transaction);
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
