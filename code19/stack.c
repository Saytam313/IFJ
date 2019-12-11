/*
*           Škola: Brno University of Technology, Faculty of Information Technology
*  Název projektu: IFJ19
*        Předměty: IFJ a IAL
*            Část: Stack
*             Tým: 015
*        Varianta: I
*          Autoři: Šimon Matyáš        (xmatya11)
*                  Marek Bahník        (xbahni00)
*                  Aliakse Slepitsa    (xslepi00)
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"

//Initialize stack
stack_t* S_Init () {

    stack_t* s;
    if( !(s = (stack_t*)malloc(sizeof(stack_t))) ){
        fprintf(stderr, "S_Init error type%d\n", 99);
    }

    s->top_ptr = NULL;
    return s;
}
//Free stack
void S_free(stack_t* s){
    free(s);
}
//Push new element to top of stack
void S_Push (stack_t *s, char* d) {

    elem_t *new_element;
    if ( (new_element = (elem_t *) malloc(sizeof(struct elem_t))) == NULL ){
        fprintf(stderr, "S_Push error type%d\n", 99);
    }


    new_element->data = d;
    new_element->next_ptr = s->top_ptr;
    s->top_ptr = new_element;
}
//Push new token to top of stack
void S_Push_Token (stack_t *s, Token_t tok) {

    elem_t *new_element;
    if ( (new_element = (elem_t *) malloc(sizeof(struct elem_t))) == NULL ){
        fprintf(stderr, "S_push_token error type%d\n", 99);
    }
    new_element->tok = tok;
    new_element->data = NULL;
    new_element->next_ptr = s->top_ptr;
    s->top_ptr = new_element;
}
//Pop top element from stack
void S_Pop (stack_t *s) {

    if ( s->top_ptr != NULL ){
        elem_t* tmp = s->top_ptr;
        s->top_ptr = s->top_ptr->next_ptr;
        free(tmp);
    }
}
//Return top element of stack
char* S_Top (stack_t *s) {

    if ( s->top_ptr != NULL ){
        return s->top_ptr->data;
    }

    else
        return NULL;
}
//Return top token of stack
Token_t S_Top_token (stack_t *s) {

    if ( s->top_ptr != NULL ){
        return s->top_ptr->tok;
    }
    else{
        Token_t tmp;
        return tmp;
    }
}
//return 0 if stack is empty else -1
int S_Empty (stack_t *s) {

    return ( s->top_ptr != NULL ? 0 : -1 );
}
//Copy stack
void S_Copy (stack_t *dst_stack, stack_t *src_stack) {

    while ( !S_Empty(src_stack) ) {
        S_Push(dst_stack, S_Top(src_stack));
        S_Pop(src_stack);
    }
}
//Print stack
void S_Print (stack_t *s) {

    elem_t *tmp = s->top_ptr;
    while ( s->top_ptr != NULL ) {
        printf ("%s\n", (s->top_ptr->data));
        S_Pop(s);
    }
    s->top_ptr = tmp;
}
