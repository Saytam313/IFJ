#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "stack.h"
#include "instr_list.h"
#include "expression.h"
#include "built_in.h"



int main()
{
    list_init();
    if(scanner_init()==0)
        return 0;


   parse();
   printf("\n\n");
   print_list();

   return 0;
}
