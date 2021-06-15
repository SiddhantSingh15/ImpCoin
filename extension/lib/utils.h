#ifndef UTILS_H
#define UTILS_H

uint64_t random_num(void);

void rand_hash(const unsigned char *in, unsigned char *out);

char *formatted_time(time_t *t);

#endif /* UTILS_H */
