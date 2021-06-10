#ifndef PARSER_H
#define PARSER_H

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

#endif // PARSER_H