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

//generuje kód pro funkci print
void builtin_print();
//generuje kod pro vestavenou funkci len
void builtin_length(char* id, char* act_func);
//generuje kod pro vestavenou funkci inputs
void builtin_inputs(char* res_var);
//generuje kod pro vestavenou funkci inputi
void builtin_inputi(char* res_var);
//generuje kod pro vestavenou funkci inputf
void builtin_inputf(char* res_var);
//generuje kod pro volani funkce chr
void builtin_chr_generate(int state);
//generuje kod pro telo funkce ord
void builtin_ord_generate(int state);
//generuje kod tela funkce substr
void builtin_substr_generate(int state);
//generuje kod pro volani funkce chr
void builtin_chr_call(SymbolTable_t *fce_list,char* res_var,char* actual_fce);
//generuje kod pro volani funkce ord
void builtin_ord_call(SymbolTable_t *fce_list,char* res_var,char* actual_fce);
//generuje kod pro volani funkce substr
void builtin_substr_call(SymbolTable_t *fce_list,char* res_var,char* actual_fce);


#endif // BUILT_IN_H_INCLUDED
