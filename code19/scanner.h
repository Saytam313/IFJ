/*
*           Škola: Brno University of Technology, Faculty of Information Technology
*  Název projektu: IFJ19
*        Předměty: IFJ a IAL
*            Část: Scanner
*             Tým: 015
*        Varianta: I
*          Autoři: Aleš Tetur          (xtetur01)
*                  Šimon Matyáš        (xmatya11)
*/


#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#include "token.h"


bool get_next_token(FILE *file, Token_t *token);

Token_t *create_new_token(void);

bool scanner_init(void);

void scanner_free(void);


#endif // SCANNER_H_INCLUDED
