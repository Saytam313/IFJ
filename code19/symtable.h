/*
*           Škola: Brno University of Technology, Faculty of Information Technology
*  Název projektu: IFJ19
*        Předměty: IFJ a IAL
*            Část: Symbol Table
*             Tým: 015
*        Varianta: I
*          Autoři: Šimon Matyáš        (xmatya11)
*                  Marek Bahník        (xbahni00)
*                  Aliakse Slepitsa    (xslepi00)
*/
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdbool.h>

typedef struct Item{
    char *key;
    struct Item *lptr;
    struct Item *rptr;
} Item_t;

typedef struct SymbolTable{
    char *key;
    int param_count;
    Item_t *top;
    struct SymbolTable *next;
} SymbolTable_t;

//find if funtion exists in symtable
bool stl_find_func(SymbolTable_t *table, char* name);
#define DEF_NODE_STACK_SIZE 2000
//initialize symbol table
void stl_init(SymbolTable_t **table);
//push new symbol to table
void stl_push(SymbolTable_t **table, char * name);
//push new symbol
bool stl_insert_top(SymbolTable_t *table, char *key);
//search for symbol in table
bool stl_search(SymbolTable_t *table, char *key, char* name);
//print symbol table
void stl_print(SymbolTable_t *table);
//insert new symbol to top
bool stl_insert_to_top(SymbolTable_t *table, char *key, char *name);
//counts number of functions in symtable
int stl_number_of_func(SymbolTable_t *table);
//finds number of parameters for function
int stl_number_of_par(SymbolTable_t *table, char* name);
#endif // SYMTABLE_H
