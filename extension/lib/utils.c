#include <stdint.h>
#include <sodium.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sodium/crypto_generichash.h>

uint64_t random_num(void) {
  uint64_t rand = ((uint64_t) (randombytes_random() | 0x0000000000000000) << 32) 
    + randombytes_random();
  return rand;
}

char *rand_hash(const unsigned char *in, size_t len) {
  char *out = malloc(crypto_generichash_BYTES);
  crypto_generichash((unsigned char *) out, crypto_generichash_BYTES, in, len, 
    NULL, 0);
  return out;
}

char *formatted_time(time_t *t) {
  char *fmttime = calloc(1, 100);
  strftime(fmttime, 100, "%m/%d/%y %X %Z", gmtime(t));
  fmttime = realloc(fmttime, strlen(fmttime) + 1);
  return fmttime;
}
