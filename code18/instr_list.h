#ifndef INSTR_LIST_H_INCLUDED
#define INSTR_LIST_H_INCLUDED
#include "parser.h"


extern struct list_t * instr_list;

typedef struct instruction_t{
	char * instr_name;
    char * operand1;
    char * operand2;
    char * operand3;
	struct instruction_t* next;
}instruction_t;


typedef struct list_t{
	instruction_t * First;
	instruction_t * Last;
}list_t;

void list_init();
void print_list();
void push_list(char * instr, char * par1, char* par2, char* par3 );

#endif // INSTR_LIST_H_INCLUDED
