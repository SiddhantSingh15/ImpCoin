#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MAX_TRANSACTIONS_PER_BLOCK 4
#define UID_LENGTH 32
#define HASH_SIZE 32

#define BUFFER_SIZE 511
#define LARGE_BUFFER_SIZE 800

#define GENESIS_TIME -22118400

#define PTR_CHECK(ptr, msg) if (!ptr) { perror(msg); exit(EXIT_FAILURE); }

// Colors
#define NOCOLOUR "\033[0m"
#define RED "\033[31m"               /* Red */
#define GREEN "\033[32m"             /* Green */
#define YELLOW "\033[33m"            /* Yellow */
#define BOLDBLUE "\033[1m\033[34m"   /* Bold Blue */
#define BOLDWHITE "\033[1m\033[37m"  /* Bold White */
#define BOLDGREEN "\033[1m\033[32m"  /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m" /* Bold Yellow */
#define BOLDRED "\033[1m\033[31m"    /* Bold Red */

#endif /* DEFINITIONS_H */
