#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

uint64_t random_long(void);

char *rand_hash(const unsigned char *in, size_t len);

char *to_hex_string_hash(char *in);

char *formatted_time(time_t *t);

void read_line(char *buffer, size_t len);

bool is_command(char c, char *buffer);

/**
 * @brief In-house string breaking function. strtok_r could not be used due to
 *        its lack of portability. strtok_r and strdup dont work on Mac OS.
 *        Another feature of this function is that it retains the delimiter in
 *        the resultant token.
 *
 * @param s The string to be tokenised.
 * @param delims String of delimiters.
 * @param save_pointer Pointer to the save destination.
 *
 * @return Resultant token from input string `s`.
 */
char *strbrk_r(char *s, const char *delims, char **save_pointer);

#endif /* UTILS_H */
