#ifndef PARSER_H
#define PARSER_H

void parse_dataproc(void *ll_node, union instr_code code, symbol_table *st);

void parse_mult(void *ll_node, union instr_code, symbol_table *st);

void parse_sdt(void *ll_node, union instr_code code, symbol_table *st);

void parse_branch(void *ll_node, union instr_code code, symbol_table *st);

#endif // PARSER_H