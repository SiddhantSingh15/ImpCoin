#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "test/test_utils.h"
#include "lib/linked_list.h"

void dummy_free(void *unused) {}

void test_ll_add_delete(int *passing, int *total) {
  linked_list *ll = ll_init();

  int a = 21;
  int b = 42;
  int c = 53;

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

  track_test(test_bool(0 == ll->size, "Size reduces on remove"), passing,
             total);

  ll_free(ll, dummy_free);
}

void test_linked_list(int *passing, int *total) {
  printf("---------------------------------------------------------------------"
         "\n");
  printf("-----%sLINKED LIST "
         "TESTS%s----------------------------------------------\n",
         BOLDBLUE, NOCOLOUR);
  int internal_passing = 0;
  int internal_total = 0;

  // test_strbrk_r(&internal_passing, &internal_total);
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

int main(void) {
  int passing = 0;
  int total = 0;

  test_linked_list(&passing, &total);

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
