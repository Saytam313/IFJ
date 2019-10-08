#include "symtable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtable.h"


void stl_init(SymbolTable_t **table)
{
    *table = (SymbolTable_t *) malloc(sizeof(SymbolTable_t));
    (*table)->key = "Main";
    (*table)->top = NULL;
    (*table)->next = NULL;
}


void stl_push(SymbolTable_t **table, char * name)
{
    SymbolTable_t *new_ST = (SymbolTable_t *) malloc(sizeof(SymbolTable_t));
    new_ST->next = (*table);
    new_ST->top = NULL;
    new_ST->param_count = 0;
    *table = new_ST;
    (*table)->key = name;
}


bool stl_insert_top(SymbolTable_t *table, char *key)
{
    if (!table)
        return false;
    if (table->top == NULL)
    {
        table->top = (Item_t *) malloc(sizeof(Item_t));
        table->top->lptr = NULL;
        table->top->rptr = NULL;
        table->top->key = key;
        return true;
    }

    Item_t *curr_item = table->top;
    int key_difference;

    while (curr_item != NULL)
    {

        key_difference = strcmp(key, curr_item->key);

        if (key_difference == 0)
        {
            return true;
        }

        else if (key_difference > 0)
        {
            if (curr_item->rptr == NULL)
            {
                curr_item->rptr = (Item_t *) malloc( sizeof(Item_t) );
                memset(curr_item->rptr, 0, sizeof(Item_t));
                curr_item = curr_item->rptr;
                break;
            }

            curr_item = curr_item->rptr;
        }

        else if (key_difference < 0)
        {
            if (curr_item->lptr == NULL)
            {
                curr_item->lptr = (Item_t *) malloc(sizeof(Item_t));
                memset(curr_item->lptr, 0, sizeof(Item_t));
                curr_item = curr_item->lptr;
                break;
            }
            curr_item = curr_item->lptr;
        }
    }

    curr_item->key = key;
    return true;
}

bool stl_search(SymbolTable_t *table, char *key, char *name)
{
    if (!table || !key)
    {
        printf("vracim nULL\n");
        return NULL;
    }

    while(strcmp(table->key, name)!= 0){
        if(table->next == NULL){
            return false;
        }
        table = table->next;
    }

    Item_t *curr_item;
    int key_difference;

    if (table != NULL)
    {
        curr_item = table->top;
        while (curr_item != NULL)
        {
            key_difference = strcmp(key, curr_item->key);
            if (key_difference == 0)
            {
                return true;
            }
            else if (key_difference > 0)
            {
                curr_item = curr_item->rptr;
            }
            else
            {
                curr_item = curr_item->lptr;
            }
        }
    }
    return false;
}

 void recursive_print(Item_t *table){
    printf("         %s\n", table->key);
    if(table->lptr!= NULL){
        recursive_print(table->lptr);
    }
    if(table->rptr!= NULL){
        recursive_print(table->rptr);
    }
 }

 void stl_print(SymbolTable_t *table){
    while((table)!= NULL){
        printf("%s\n", (table)->key);
        if((table)->top!=NULL){
            recursive_print((table)->top);
        }
        table = (table)->next;
    }
 }

bool stl_insert_to_top(SymbolTable_t *table, char *key, char *name){
    if (!table){
        return false;
    }
    while(strcmp(table->key, name)!= 0){
        if(table->next == NULL){
            return false;
        }
        table = table->next;
    }
    if (table->top == NULL)
    {
        table->top = (Item_t *) malloc(sizeof(Item_t));
        table->top->lptr = NULL;
        table->top->rptr = NULL;
        table->top->key = key;
        return true;
    }


    Item_t *curr_item = table->top;
    int key_difference;

    while (curr_item != NULL)
    {

        key_difference = strcmp(key, curr_item->key);

        if (key_difference == 0)
        {
            return true;
        }
        else if (key_difference > 0)
        {
            if (curr_item->rptr == NULL)
            {
                curr_item->rptr = (Item_t *) malloc( sizeof(Item_t) );
                memset(curr_item->rptr, 0, sizeof(Item_t));
                curr_item = curr_item->rptr;
                break;
            }

            curr_item = curr_item->rptr;
        }
        else if (key_difference < 0)
        {
            if (curr_item->lptr == NULL)
            {
                curr_item->lptr = (Item_t *) malloc(sizeof(Item_t));
                memset(curr_item->lptr, 0, sizeof(Item_t));
                curr_item = curr_item->lptr;
                break;
            }
            curr_item = curr_item->lptr;
        }
    }
    curr_item->key = key;
    return true;
}

bool stl_find_func(SymbolTable_t *table, char* name){
    while(table->next != NULL){
        if(strcmp(table->key, name) == 0){
            return true;
        }
        table = table->next;
    }
    return false;
}

int stl_number_of_func(SymbolTable_t *table){
    int vysledek = 0;
    while(table->next != NULL){
        vysledek++;
        table = table->next;
    }
    return vysledek;
}

int stl_number_of_par(SymbolTable_t *table, char* name){
    while(table->next != NULL){
        if(strcmp(table->key, name) == 0){
            return table->param_count;
        }
        table = table->next;
    }
    return -1;
}
