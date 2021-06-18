#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * @brief Returns a random long integer value.
 *
 * @return The random long value.
 */
uint64_t random_long(void);

/**
 * @brief Returns a randomized string of characters to act as a hash, given a 
 *  particular string input and the length of the string. The output string/
 *  hash is allocated space in memory using malloc.
 *
 * @param in The input string to be hashed.
 * @param len The length of the given string 'in'.
 *
 * @return The random string of characters to act as a hash.
 */
char *rand_hash(const unsigned char *in, size_t len);

/**
 * @brief Uses the libsodium library to convert the binary to a hex string.
 *
 * @param in The input binary.
 *
 * @return The return char * which contains the hex conversion.
 */
char *to_hex_string_hash(char *in);

/**
 * @brief Creates (using calloc), initializes and returns the string/printable
 *  form of a given time.
 * 
 * @param t The time to be formatted/made printable.
 *
 * @return The formatted version of the given time as a string.
 */
char *formatted_time(time_t *t);

/**
 * @brief Inputs a buffer string and the length of the string tot set each 
 *  character in the buffer string with a random character retuend by the 
 *  getchar() method.
 *
 * @param buffer The buffer string whose characters are to be set.
 * @param len The length of the buffer string.
 */
void read_line(char *buffer, size_t len);

/**
 * @brief Ensures that the input char * is a valid command.
 *
 * @param c The input character
 * @param buffer The input command
 *
 * @return Returns true if the buffer is valid.
 */
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
