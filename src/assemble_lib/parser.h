#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

/**
 * @brief Parses and encodes Data Processing Instructions.
 * 
 * @param ll_node Linked list node containing token list.
 * @param code Special flags which distinguish each instruction from each other.
 * @param st The symbol table.
 * 
 * @return The parsed and encoded 32 bit binary.
 */
uint32_t parse_dataproc(void *ll_node, union instr_code code, symbol_table *st);

/**
 * @brief Parses and encodes Multiplication Instructions.
 * 
 * @param ll_node Linked list node containing token list.
 * @param code Special flags which distinguish each instruction from each other.
 * @param st The symbol table.
 * 
 * @return The parsed and encoded 32 bit binary.
 */
uint32_t parse_mult(void *ll_node, union instr_code, symbol_table *st);

/**
 * @brief Parses and encodes Single Data Transfer Instructions.
 * 
 * @param ll_node Linked list node containing token list.
 * @param code Special flags which distinguish each instruction from each other.
 * @param st The symbol table.
 * 
 * @return The parsed and encoded 32 bit binary.
 */
uint32_t parse_sdt(void *ll_node, union instr_code code, symbol_table *st);

/**
 * @brief Parses and encodes Branch Instructions.
 * 
 * @param ll_node Linked list node containing token list.
 * @param code Special flags which distinguish each instruction from each other.
 * @param st The symbol table.
 * 
 * @return The parsed and encoded 32 bit binary.
 */
uint32_t parse_branch(void *ll_node, union instr_code code, symbol_table *st);

/**
 * @brief Parses and encodes special LSL Instruction.
 * 
 * @param ll_node Linked list node containing token list.
 * @param code Special flags which distinguish each instruction from each other.
 * @param st The symbol table.
 * 
 * @return The parsed and encoded 32 bit binary.
 */
uint32_t parse_lsl(void *ll_node, union instr_code code, symbol_table *st);

/**
 * @brief Taks token position and line number and prints an error.
 * 
 * @param pos Token position.
 * @param line Line number.
 */
void throw_parse_error(uint8_t pos, uint32_t line);

/**
 * @brief Takes a bool condition, position and line. If condition is false,
 *        throws an error.
 * 
 * @param token_cond Token condition.
 * @param line Line number.
 * 
 * @return True if condition is satisfied.
 */
bool assert_token(bool token_cond, uint8_t pos, uint32_t line);

/**
 * @brief Ensures the given token matches the seperator. If this doesnt hold, 
 *        program will exit.
 * 
 * @param tok Token.
 * @param seperator Seperator character.
 * @param pos Token position.
 * @param line Line position.
 * 
 * @return True if token and seperator character are equal, exits program if
 *         not.
 */
bool assert_separator(struct token tok, char separator, uint8_t pos,
                      uint8_t line);


/**
 * @brief Increments pointer if the condition is true.
 * 
 * @param cond Given condition.
 * @param pos Pointer to position.
 * 
 * @return Returns true if cond is true.
 */
bool cond_move(bool cond, uint8_t *pos);

/**
 * @brief Checks if token and seperator are equal.
 * 
 * @param tok Token. 
 * @param separator Seperator character.
 * 
 * @return Returns true if token character is equal to seperator character.
 */
bool match_separator(struct token tok, char separator);

/**
 * @brief Asserts if token type is the expected one. If the assert fails, 
 *        program exits. If it passes, cond_move is called.
 * 
 * @param tok Token.
 * @param expected Expected token type.
 * @param pos Pointer to position.
 * @param line Line number.
 * 
 * @return Returns the data from the token. 
 */
union token_data get_and_move(struct token tok, enum token_type expected,
                               uint8_t *pos, uint8_t line);

/**
 * @brief Asserts if token type is the expected one. If the assert fails, 
 *        program exits. If it passes, cond_move isn't called.
 * 
 * @param tok Token.
 * @param expected Expected token type.
 * @param pos Pointer to position.
 * @param line Line number.
 * 
 * @return Returns the data from the token. 
 */
union token_data match_and_get(struct token tok, enum token_type expected,
                               uint8_t pos, uint8_t line);

#endif // PARSER_H
