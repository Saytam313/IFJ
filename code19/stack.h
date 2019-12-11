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
//Initialize stack
stack_t* S_Init ();
//Free stack
void S_free(stack_t* s);
//Push new element to top of stack
void S_Push (stack_t *s, char* data);
//Push new token to top of stack
void S_Push_Token(stack_t *s, Token_t tok);
//Pop top element from stack
void S_Pop (stack_t *s);
//Return top element of stack
char* S_Top (stack_t *s);
//Return top token of stack
Token_t S_Top_token (stack_t *s);
//return 0 if stack is empty else -1
int S_Empty (stack_t *s);
//Copy stack
void S_Print (stack_t *s);
//Print stack
void S_Copy (stack_t *dst_stack, stack_t *src_stack);


#endif
