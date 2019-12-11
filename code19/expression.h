/*
*           Škola: Brno University of Technology, Faculty of Information Technology
*  Název projektu: IFJ19
*        Předměty: IFJ a IAL
*            Část: Expression
*             Tým: 015
*        Varianta: I
*          Autoři: Šimon Matyáš        (xmatya11)
*                  Marek Bahník        (xbahni00)
*                  Aliakse Slepitsa    (xslepi00)
*/
#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "stack.h"
#include "instr_list.h"
#include "symtable.h"


//evaluating expression
void expression(char* act_func, char* tok);

#endif // EXPRESSION_H_INCLUDED
