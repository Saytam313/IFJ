#ifndef SYMTABLE_H
#define SYMTABLE_H

 #include <stdio.h>
    #include <stdbool.h>

    typedef struct Item
    {
        char *key;
        struct Item *lptr;
        struct Item *rptr;
    } Item_t;

    typedef struct SymbolTable
    {
        char *key;
        int param_count;
        Item_t *top;
        struct SymbolTable *next;
    } SymbolTable_t;


    bool stl_find_func(SymbolTable_t *table, char* name);
    #define DEF_NODE_STACK_SIZE 2000

    void stl_init(SymbolTable_t **table);
    void stl_push(SymbolTable_t **table, char * name);
    bool stl_insert_top(SymbolTable_t *table, char *key);
    bool stl_search(SymbolTable_t *table, char *key, char* name);
    void stl_print(SymbolTable_t *table);
    bool stl_insert_to_top(SymbolTable_t *table, char *key, char *name);
    int stl_number_of_func(SymbolTable_t *table);
    int stl_number_of_par(SymbolTable_t *table, char* name);
#endif // SYMTABLE_H
