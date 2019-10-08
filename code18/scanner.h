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
