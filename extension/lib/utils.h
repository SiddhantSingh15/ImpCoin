#ifndef UTILS_H
#define UTILS_H

#define PTR_CHECK(ptr, msg) if (!ptr) { perror(msg); exit(EXIT_FAILURE); }

uint64_t random_long(void);

char *rand_hash(const unsigned char *in, size_t len);

char *to_hex_string_hash(char *in);

char *formatted_time(time_t *t);

#endif /* UTILS_H */
