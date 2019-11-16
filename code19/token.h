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

#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//Hodnoty, kter� m��e token nab�vat
typedef enum {
    token_nic,              // 0

    token_chr,              // 1
    token_def,              // 2
    token_do,               // 3
    token_else,             // 4
    token_end,              // 5
    token_if,               // 6
    token_inputs,           // 7
    token_inputi,           // 8
    token_inputf,           // 9
    token_length,           // 10
    token_nil,              // 11
    token_ord,              // 12
    token_print,            // 13
    token_substr,           // 14
    token_then,             // 15
    token_while,            // 16
    token_doc_string,       // 17
    token_pass,             // 18
    token_return,           // 19

    token_plus,             // 20
    token_minus,            // 21
    token_mul,              // 22
    token_div,              // 23

    token_less_equal,       // 24
    token_less,             // 25
    token_great_equal,      // 26
    token_great,            // 27
    token_neg_equal,        // 28
    token_equal_equal,      // 29

    token_left_bracket,     // 30
    token_right_bracket,    // 31
    token_comma,            // 32
    token_equal,            // 33

    token_eol,              // 34
    token_eof,              // 35

    token_id,               // 36
    token_val_int,          // 37
    token_val_float,        // 38
    token_string,           // 39

    token_indent,           // 40
    token_dedent,           // 41

    token_test

} token_typ;

//Mo�n� typy ��seln�ch prom�nn�ch
typedef union {
    int i;
    float d;
    char *c;
} token_val;

//Struktura tokenu
typedef struct TOKEN {
    token_typ type;
    token_val val;
    unsigned int line;
} Token_t;

#endif
