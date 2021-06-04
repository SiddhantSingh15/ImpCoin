#ifndef PARSER_H
#define PARSER_H

uint32_t parse_dataproc(void *ll_node, union instr_code code, symbol_table *st);

uint32_t parse_mult(void *ll_node, union instr_code, symbol_table *st);

uint32_t parse_sdt(void *ll_node, union instr_code code, symbol_table *st);

uint32_t parse_branch(void *ll_node, union instr_code code, symbol_table *st);

#endif // PARSER_H