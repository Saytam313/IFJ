/*
*           Škola: Brno University of Technology, Faculty of Information Technology
*  Název projektu: IFJ19
*        Předměty: IFJ a IAL
*            Část: Instructions
*             Tým: 015
*        Varianta: I
*          Autoři: Šimon Matyáš        (xmatya11)
*                  Marek Bahník        (xbahni00)
*                  Aliakse Slepitsa    (xslepi00)
*/
#ifndef INSTR_LIST_H_INCLUDED
#define INSTR_LIST_H_INCLUDED
#include "parser.h"


extern struct list_t * instr_list;
//instruction structure, represents one line of IFJcode19
typedef struct instruction_t{
	char * instr_name;
    char * operand1;
    char * operand2;
    char * operand3;
	struct instruction_t* next;
}instruction_t;

//list of instructions
typedef struct list_t{
	instruction_t * First;
	instruction_t * Last;
}list_t;

//initialization of list
void list_init();

//generates IFJcode19
void print_list();
//adds list new list element
void push_list(char * instr, char * par1, char* par2, char* par3 );

#endif // INSTR_LIST_H_INCLUDED
