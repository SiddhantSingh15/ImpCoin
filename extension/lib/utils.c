#include <sodium/crypto_generichash.h>
#include <stdint.h>
#include <sodium.h>

uint64_t random_num(void) {
  uint64_t rand = ((uint64_t) (randombytes_random() | 0x0000000000000000) << 32) + randombytes_random();
  return rand;
}

void rand_hash(const unsigned char *in, unsigned char *out) {
   crypto_generichash(out, crypto_generichash_BYTES, in, sizeof(in), NULL, 0);
}
