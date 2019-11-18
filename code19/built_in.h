#ifndef BUILT_IN_H_INCLUDED
#define BUILT_IN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "stack.h"
#include "instr_list.h"
#include "symtable.h"
#include "expression.h"

void builtin_print();
void builtin_length(char* id, char* act_func);
void builtin_inputs(char* res_var);
void builtin_inputi(char* res_var);
void builtin_inputf(char* res_var);
void builtin_chr_generate(int state);
void builtin_ord_generate(int state);
void builtin_substr_generate(int state);
void builtin_chr_call(SymbolTable_t *fce_list,char* res_var,char* actual_fce);
void builtin_ord_call(SymbolTable_t *fce_list,char* res_var,char* actual_fce);
void builtin_substr_call(SymbolTable_t *fce_list,char* res_var,char* actual_fce);


#endif // BUILT_IN_H_INCLUDED
