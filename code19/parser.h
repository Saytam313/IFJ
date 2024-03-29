/*
*           Škola: Brno University of Technology, Faculty of Information Technology
*  Název projektu: IFJ19
*        Předměty: IFJ a IAL
*            Část: Parser
*             Tým: 015
*        Varianta: I
*          Autoři: Aleš Tetur          (xtetur01)
*                  Šimon Matyáš        (xmatya11)
*                  Marek Bahník        (xbahni00)
*                  Aliakse Slepitsa    (xslepi00)
*/
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
//start of code parsing
void parse();

//concatenates 2 strings
char* concat(char* str1, char* str2);

//returns name of var in IFJcode19 form
char* get_var_name(char* name);

#endif // PARSER_H_INCLUDED
