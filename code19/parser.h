#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.h"
#include "stack.h"
#include "instr_list.h"
#include "symtable.h"
#include "expression.h"

Token_t * token;
FILE* f;
stack_t *label_Stack;
SymbolTable_t* tabulka;
Token_t last_token;
void parse();
char* concat(char* str1, char* str2);
char* get_var_name(char* name);
#endif // PARSER_H_INCLUDED
