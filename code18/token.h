///////////////////////////////////////////////////////////////////////////////////
// Škola:       Brno University of Technology, Faculty of Information Technology //
// Pøedmìt:     IFJ                                                              //
// Projekt:     IFJ18                                                            //
// Èást:        Scanner                                                          //
// Autoøi:      Aleš Tetur          (xtetur01)                                   //
//              Šimon Matiáš        (xmatya11)                                   //
//              Jan Tilgner         (xtilgn01)                                   //
//              Jan Bíl             (xbilja00)                                   //
///////////////////////////////////////////////////////////////////////////////////
#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//Hodnoty, které mùže token nabívat
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
    token_block_beg,        // 17
    token_block_end,        // 18

    token_plus,             // 19
    token_minus,            // 20
    token_mul,              // 21
    token_div,              // 22

    token_less_equal,       // 23
    token_less,             // 24
    token_great_equal,      // 25
    token_great,            // 26
    token_neg_equal,        // 27
    token_equal_equal,      // 28

    token_left_bracket,     // 29
    token_right_bracket,    // 30
    token_comma,            // 31
    token_equal,            // 32

    token_eol,              // 33
    token_eof,              // 34

    token_id_function,      // 35
    token_id,               // 36
    token_val_int,          // 37
    token_val_float,        // 38
    token_string,           // 39

    token_test

} token_typ;

//Možné typy èíselných promìnných
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
