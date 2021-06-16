#include <stddef.h>
#include <stdint.h>
#include <sodium.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sodium/crypto_generichash.h>
#include <sodium/utils.h>

uint64_t random_long(void) {
  return ((uint64_t)randombytes_random() << 32) | randombytes_random();
}

char *rand_hash(const unsigned char *in, size_t len) {
  char *out = malloc(crypto_generichash_BYTES);
  crypto_generichash((unsigned char *) out, crypto_generichash_BYTES, in, len,
    NULL, 0);
  return out;
}

char *to_hex_string_hash(char *in) {
  char *hex = malloc(crypto_generichash_BYTES * 2 + 1);
  return sodium_bin2hex(hex, crypto_generichash_BYTES * 2 + 1, (unsigned char *)in,
                        crypto_generichash_BYTES);
}

char *formatted_time(time_t *t) {
  char *fmttime = calloc(1, 100);
  strftime(fmttime, 100, "%m/%d/%y %X %Z", gmtime(t));
  fmttime = realloc(fmttime, strlen(fmttime) + 1);
  return fmttime;
}

void read_line(char *buffer, size_t len) {
  for (int i = 0; i < len; i++) {
    char c = getchar();
    if (c == '\n') {
      buffer[i] = '\0';
      break;
    };
    buffer[i] = c;
  }
}
