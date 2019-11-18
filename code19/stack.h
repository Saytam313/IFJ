#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "token.h"

typedef struct elem_t {
    char* data;
    Token_t tok;
    struct elem_t *next_ptr;
} elem_t;

typedef struct stack_t {
    elem_t *top_ptr;
} stack_t;

stack_t* S_Init ();
void S_free(stack_t* s);
void S_Push (stack_t *s, char* data);
void S_Push_Token(stack_t *s, Token_t tok);
void S_Pop (stack_t *s);
char* S_Top (stack_t *s);
Token_t S_Top_token (stack_t *s);
int S_Empty (stack_t *s);
void S_Print (stack_t *s);
void S_Copy (stack_t *dst_stack, stack_t *src_stack);


#endif
