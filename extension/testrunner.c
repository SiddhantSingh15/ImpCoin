#include "test/test_utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int passing = 0;
  int total = 0;
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
