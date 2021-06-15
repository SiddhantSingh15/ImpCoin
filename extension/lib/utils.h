#ifndef UTILS_H
#define UTILS_H

uint64_t random_long(void);

char *rand_hash(const unsigned char *in, size_t len);

char *to_hex_string_hash(char *in);

char *formatted_time(time_t *t);

#endif /* UTILS_H */
