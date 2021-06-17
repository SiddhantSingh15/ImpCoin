#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sodium.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include <sodium/crypto_generichash.h>
#include <sodium/utils.h>

#include "../definitions.h"

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

bool is_command(char c, char *buffer) {
  return buffer[0] == c && (buffer[1] == ' ' || !buffer[1]);
}

/*
  s      = "[r1, r2]"
  delims = "[], "
  save_pointer = &s

  token 1: [        0       s: r1, r2]
  token 2: r1       2       s: , r2]
  token 3: ,        0       s:  r2]
  token 4:          0       s: r2]
  token 5: r2       2       s: ]
  token 6: ]        0       s:
*/
char *strbrk_r(char *s, const char *delims, char **save_pointer) {

  if (s == NULL) {
    s = *save_pointer;
  }

  if (*s == '\0') {
    return NULL;
  }

  char *token = malloc(strlen(s) + 1);
  PTR_CHECK(token, "Memory allocation failure\n");
  int tok_size = strcspn(s, delims);
  tok_size = tok_size == 0 ? 1 : tok_size;

  assert(tok_size <= strlen(s));
  *token = '\0';
  strncat(token, s, tok_size);
  token = realloc(token, sizeof(char) * (strlen(token) + 1));
  PTR_CHECK(token, "Memory allocation failure\n");

  *save_pointer += tok_size;

  return token;
}
