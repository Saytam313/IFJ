#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "stack.h"
#include "instr_list.h"

list_t * instr_list;


void list_init()
{
	instr_list = (list_t*) malloc(sizeof(list_t));
	if(!instr_list){
        fprintf(stderr, "error type: %d\n", 99);
	}
	instr_list->First=NULL;
	instr_list->Last=NULL;
}

instruction_t * instr_init (){
	instruction_t * new_instr =  (instruction_t *)malloc(sizeof(instruction_t));
	if(!new_instr){
        fprintf(stderr, "error type: %d\n", 99);
	}
	new_instr->instr_name = (char *) malloc(12*sizeof(char));
	new_instr->operand1 = new_instr->operand2 = new_instr->operand3 = NULL;
	new_instr->next = NULL;

	return new_instr;
}


void push_list(char * instr, char * par1, char * par2, char * par3 )
{
	instruction_t * new_instr = instr_init();
	strcpy(new_instr->instr_name,instr);
	if(par1 != NULL){
        new_instr->operand1 = par1;
	} else {
	    new_instr->operand1 = NULL;
    }

	if(par2 != NULL){
        new_instr->operand2 = par2;
	} else {
        new_instr->operand2 = NULL;
	}

	if(par3 != NULL){
        new_instr->operand3 = par3;
	} else {
        new_instr->operand3 = NULL;
	}

	if(instr_list){
		if(instr_list->First == NULL)
			instr_list->First=instr_list->Last=new_instr;
		else {
			instr_list->Last->next=new_instr;
			instr_list->Last=new_instr;
		}
	}
}


void print_list(){
	printf(".IFJcode18\n");
    instruction_t * tmp = NULL;
	for(tmp = instr_list->First; tmp!=NULL; tmp=tmp->next){
        if (tmp->instr_name) printf("%s",tmp->instr_name);
        if(tmp->operand1){
            printf(" ");
            printf("%s", tmp->operand1);
        }

        if(tmp->operand2){
            printf(" ");
            printf("%s", tmp->operand2);
        }
        if(tmp->operand3){
            printf(" ");
            printf("%s", tmp->operand3);
        }
        printf("\n");
    }
}
