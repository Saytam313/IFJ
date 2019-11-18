#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"


stack_t* S_Init () {

    stack_t* s;
    if( !(s = (stack_t*)malloc(sizeof(stack_t))) ){
        fprintf(stderr, "S_Init error type%d\n", 99);
    }

    s->top_ptr = NULL;
    return s;
}
void S_free(stack_t* s){
    free(s);
}

void S_Push (stack_t *s, char* d) {

    elem_t *new_element;
    if ( (new_element = (elem_t *) malloc(sizeof(struct elem_t))) == NULL ){
        fprintf(stderr, "S_Push error type%d\n", 99);
    }


    new_element->data = d;
    new_element->next_ptr = s->top_ptr;
    s->top_ptr = new_element;
}

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

void S_Pop (stack_t *s) {

    if ( s->top_ptr != NULL ){
        elem_t* tmp = s->top_ptr;
        s->top_ptr = s->top_ptr->next_ptr;
        free(tmp);
    }
}

char* S_Top (stack_t *s) {

    if ( s->top_ptr != NULL ){
        return s->top_ptr->data;
    }

    else
        return NULL;
}
Token_t S_Top_token (stack_t *s) {

    if ( s->top_ptr != NULL ){
        return s->top_ptr->tok;
    }
    else{
        Token_t tmp;
        return tmp;
    }

}

int S_Empty (stack_t *s) {

    return ( s->top_ptr != NULL ? 0 : -1 );
}

void S_Copy (stack_t *dst_stack, stack_t *src_stack) {

    while ( !S_Empty(src_stack) ) {
        S_Push(dst_stack, S_Top(src_stack));
        S_Pop(src_stack);
    }
}

void S_Print (stack_t *s) {

    elem_t *tmp = s->top_ptr;
    while ( s->top_ptr != NULL ) {
        printf ("%s\n", (s->top_ptr->data));
        S_Pop(s);
    }
    s->top_ptr = tmp;
}
