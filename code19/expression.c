#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "expression.h"
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "stack.h"
#include "instr_list.h"
#include "symtable.h"
#include "built_in.h"

char precedence_table [13][13] = {
/*            +    -    *    /    <=   <   >=    >   !=    ==   (    )    $ */
/*  +  */   {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '>'},
/*  -  */   {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '>'},
/*  *  */   {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '>'},
/*  /  */   {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '>'},
/* <=  */   {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '>'},
/*  <  */   {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '>'},
/* >=  */   {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '>'},
/*  >  */   {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '>'},
/* !=  */   {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '>'},
/* ==  */   {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '>'},
/*  (  */   {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', ' '},
/*  )  */   {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', ' ', '>', '>'},
/*  $  */   {'<', '<', '<', '<', '<', ' ', '<', '<', '<', '<', '<', ' ', ' '},
};

char* get_float_name(float n);

char* get_int_name(int n);

char* get_name(Token_t tok){
    char* vysledek = (char *)malloc(20000);
    switch(tok.type){
    case token_id:
        vysledek = get_var_name(tok.val.c);
        break;
    case token_val_int:
        vysledek = get_int_name(tok.val.i);
        break;

    case token_val_float:
        vysledek = get_float_name(tok.val.d);
        break;
    case token_string:
        vysledek = concat("string@", tok.val.c);
        break;
    case token_nil:
        strcpy(vysledek,"nil@nil");
        break;
    default:
        break;
    }
    vysledek=realloc(vysledek,strlen(vysledek));
    return vysledek;
}

char* str_num(char* s, int d){
    char* vysledek = (char *)malloc(strlen(s)+sizeof(int));
    sprintf(vysledek, "%s%d", s, d);
    return vysledek;
}

//returns IFJcode19 code for int
char* get_int_name(int n){
    int delka;
    if(n == 0){
        delka = 1;
    } else{
        delka = (int)(log10(n)+1);
    }
    char* vysledek = (char *)malloc(delka * sizeof(char) + 5);
    sprintf(vysledek, "int@%d", n);
    return vysledek;
}
//returns IFJcode19 code for float
char* get_float_name(float n){
    char* vysledek = (char *)malloc(20 * sizeof(char)+5);
    sprintf(vysledek, "float@%a", n);
    return vysledek;
}

//returns IFJcode19 code for param
char* get_par_name(int n){
    int delka = (int)(log10(n)+1);
    char* vysledek = (char *)malloc(delka * sizeof(char) + 5);
    sprintf(vysledek, "TF@%%%d", n);
    return vysledek;
}
//compare tokens using precedence_table
bool compare_op(Token_t top){
    int y = token->type - token_plus;
    int x = top.type - token_plus;

    if(precedence_table[x][y] == '<'){
        return true;
    }
    return false;
}
//check if token is builtin function
bool is_builtin_f(){
    if((token->type >= token_inputs && token->type <= token_substr && token->type != token_nil )||token->type==token_chr){
        return true;
    }
    return false;
}
//check if token is operand
bool is_operand(){
    if(token->type >= token_id && token->type <= token_string || token->type == token_nil){
        return true;
    }
    return false;
}

void oper(stack_t* output_stack, stack_t* operator_stack){
    if(S_Empty(operator_stack) || compare_op((S_Top_token(operator_stack)))){
        S_Push_Token(operator_stack, *token);
    } else {
        while(!S_Empty(operator_stack) && !compare_op(S_Top_token(operator_stack))){
            Token_t tmp = S_Top_token(operator_stack);
            S_Pop(operator_stack);
            S_Push_Token(output_stack, tmp);
        }
        S_Push_Token(operator_stack, *token);
    }
}
//clears stack between parentheses
void righ_parenth(stack_t* output_stack, stack_t* operator_stack){
    while(!S_Empty(operator_stack)){
        if(S_Top_token(operator_stack).type == token_left_bracket){
            S_Pop(operator_stack);
            break;
        }
        Token_t tmp = S_Top_token(operator_stack);
        S_Pop(operator_stack);
        S_Push_Token(output_stack, tmp);
    }
}
//check if token is arithmetic operator
bool is_arithmetic_operator(){
    if(token->type >= token_plus && token->type <= token_div_div){
        return true;
    }
    return false;
}
//check if token if logic operator
bool is_logic_operator(){
    if(token->type >= token_less_equal && token->type <= token_equal_equal){
        return true;
    }
    return false;
}
//calls function for generating builtin function
void builtin_f(char* act_func, char* id){
    switch (token->type){
        case token_print:
            get_next_token(f, token);
            builtin_print(act_func);
            break;
        case token_inputf:
            builtin_inputf(id);
            get_next_token(f, token);
            break;
        case token_inputi:
            builtin_inputi(id);
            get_next_token(f, token);
            break;
        case token_inputs:
            builtin_inputs(id);
            get_next_token(f, token);
            break;
        case token_length:
            get_next_token(f, token);
            builtin_length(id, act_func);
            break;
        case token_substr:
            get_next_token(f, token);
            builtin_substr_call(tabulka, id, act_func);
            break;
        case token_ord:
            get_next_token(f, token);
            builtin_ord_call(tabulka, id, act_func);
            break;
        case token_chr:
            get_next_token(f, token);
            builtin_chr_call(tabulka, id, act_func);
            break;
        default:
            break;
    }
    return;
}
//generating code for user function
void user_f(char* act_func, char* id){
    push_list("CREATEFRAME", NULL, NULL, NULL);
    char* func = token->val.c;
    int count_of_params = stl_number_of_par(tabulka, func);
    bool parenth = false;
    get_next_token(f, token);
    if(token->type == token_left_bracket){
        parenth = true;

        get_next_token(f, token);
    }
    int i = 1;
    while(i < count_of_params+1){
        if(token->type == token_val_int){
            push_list("DEFVAR", get_par_name(i), NULL, NULL);
            push_list("MOVE", get_par_name(i), get_int_name(token->val.i), NULL);
        } else if(token->type == token_val_float){
            push_list("DEFVAR", get_par_name(i), NULL, NULL);
            push_list("MOVE", get_par_name(i), get_float_name(token->val.d), NULL);
        } else if(token->type == token_string){
            push_list("DEFVAR", get_par_name(i), NULL, NULL);
            push_list("MOVE", get_par_name(i), concat("string@" ,token->val.c), NULL);
        } else if(token->type == token_id){
            if(!stl_search(tabulka, token->val.c, act_func)){
               fprintf(stderr, "1. user_f error type: %d\n", 3);
               exit(3);
               return;
            }
            push_list("DEFVAR", get_par_name(i), NULL, NULL);
            push_list("MOVE", get_par_name(i), concat("LF@" ,token->val.c), NULL);
        } else if(token->type == token_nil){
            push_list("DEFVAR", get_par_name(i), NULL, NULL);
            push_list("MOVE", get_par_name(i), "nil@nil", NULL);
        } else if(token->type == token_right_bracket){
            break;
        } else {
            fprintf(stderr, "%d\n", 2);
            exit(2);
            return;
        }
        get_next_token(f, token);
        if(token->type != token_comma){
            i++;
            break;
        }
        get_next_token(f, token);
        i++;
    }

    if(parenth){
        if(token->type == token_right_bracket){
           get_next_token(f, token);
        } else if(token->type == token_val_int && count_of_params == 0)  {
            fprintf(stderr, "1. rand_error (badcall4) %d\n", 5);
            exit(5);
            return;
        }   else {
            fprintf(stderr, "2. rand_error %d\n", 2);
            exit(2);
            return;
        }
    }
    if(i != count_of_params+1){
        fprintf(stderr, "2. user_f error type: %d\n", 5);
        exit(5);
        return;
    }
    if(token->type != token_eol){
        fprintf(stderr, "3. rand_error %d\n", 3);
        exit(3);
        return;
    }
    push_list("CALL", func, NULL, NULL);
    if(id != NULL){
        push_list("MOVE", concat("LF@", id), "TF@%retval", NULL);
    }
}

void postfix_instruction(stack_t* postfix_stack, char* act_func, bool logic){

    Token_t act_token = S_Top_token(postfix_stack);
    Token_t operand1;
    Token_t operand2;
    Token_t operator_tok;
    static int tmp_count = 1;
    static int sametype_count = 1;
    static int float_count = 1;
    static int div_count = 1;
    static int concat_count = 1;
    int oper1_val_type=0;
    int oper2_val_type=0;

    if( (act_token.type >= token_plus && act_token.type <= token_div_div) ||
        (act_token.type >= token_less_equal && act_token.type <= token_equal_equal)){
        operator_tok = act_token;
        S_Pop(postfix_stack);
        act_token = S_Top_token(postfix_stack);
    }

    
    if( act_token.type >= token_id && act_token.type <= token_string || act_token.type == token_nil ){
        operand1 = act_token;
        S_Pop(postfix_stack);
        if(S_Empty(postfix_stack)){
            push_list("PUSHS", get_name(act_token), NULL, NULL);
            return;
        }
        oper1_val_type=1;

        act_token = S_Top_token(postfix_stack);
    } else {
        postfix_instruction(postfix_stack, act_func, logic);
        oper1_val_type=2;



        operand1.val.c = str_num("$tmp", tmp_count);
        operand1.type = token_id;
        tmp_count++;
        act_token = S_Top_token(postfix_stack);
    }

    if( act_token.type >= token_id && act_token.type <= token_string || act_token.type == token_nil ){
        operand2 = act_token;
        S_Pop(postfix_stack);
        oper2_val_type=1;
        
        act_token = S_Top_token(postfix_stack);
    } else {
        postfix_instruction(postfix_stack, act_func, logic);
        oper2_val_type=2;



        operand2.val.c = str_num("$tmp", tmp_count);
        operand2.type = token_id;
        tmp_count++;
        act_token = S_Top_token(postfix_stack);
    }

    if(oper2_val_type==1){
        push_list("PUSHS", get_name(operand2), NULL, NULL);
    }else if(oper2_val_type==2){
        push_list("DEFVAR", str_num("LF@$tmp", tmp_count), NULL, NULL);
        push_list("POPS", str_num("LF@$tmp", tmp_count), NULL, NULL);
        push_list("PUSHS", str_num("LF@$tmp", tmp_count), NULL, NULL);
    }
    if(oper1_val_type==1){
        push_list("PUSHS", get_name(operand1), NULL, NULL);
    }else if(oper1_val_type==2){
        push_list("DEFVAR", str_num("LF@$tmp", tmp_count), NULL, NULL);
        push_list("POPS", str_num("LF@$tmp", tmp_count), NULL, NULL);
        push_list("PUSHS", str_num("LF@$tmp", tmp_count), NULL, NULL);
    }

    push_list("TYPE", "GF@$type1", get_name(operand1), NULL);
    push_list("TYPE", "GF@$type2", get_name(operand2), NULL);
    push_list("JUMPIFEQ", str_num("$sametype", sametype_count), "GF@$type1", "GF@$type2");
    push_list("JUMPIFEQ", str_num("$sametype", sametype_count), "GF@$type1", "string@nil");
    push_list("JUMPIFEQ", str_num("$sametype", sametype_count), "GF@$type2", "string@nil");
    push_list("JUMPIFEQ", "$error4", "GF@$type1", "string@string");
    push_list("JUMPIFEQ", "$error4", "GF@$type2", "string@string");

    Token_t old_operand;
    if(operand1.type != operand2.type && operand1.type != token_string && operand2.type != token_string){
        
        push_list("DEFVAR", str_num("LF@$floattmp", tmp_count), NULL, NULL);
        
        if(operand1.type == token_val_int){
            old_operand = operand1;
            
            push_list("INT2FLOATS", NULL, NULL, NULL);
            push_list("MOVE","GF@$type1","string@float",NULL);
            operand1.val.c = str_num("$floattmp", tmp_count);
            operand1.type = token_id;
            push_list("POPS", get_name(operand1),NULL, NULL);
            push_list("PUSHS", get_name(operand1),NULL, NULL);
        
        }else{
            old_operand = operand2;

            push_list("POPS", "GF@$result",NULL, NULL);
            push_list("INT2FLOATS", NULL, NULL, NULL);
            push_list("MOVE","GF@$type2","string@float",NULL);
            push_list("PUSHS", "GF@$result",NULL, NULL);
            operand2.val.c = str_num("$floattmp", tmp_count);
            operand2.type = token_id;


        }
        tmp_count++;
    }
        push_list("LABEL", str_num("$sametype", sametype_count), NULL, NULL);

    float_count++;
    sametype_count++;

    static int concatTmpVar=0;
    switch(operator_tok.type){
    case token_plus:
        if((operand1.type == token_val_float || operand1.type == token_val_int) || (operand2.type == token_val_float || operand2.type == token_val_int)) {
            push_list("ADDS", NULL, NULL,NULL);
        } else {
            push_list("JUMPIFEQ", str_num("$concat", concat_count), "GF@$type1", "string@string");
            push_list("ADDS", NULL, NULL,NULL);
            push_list("JUMP", str_num("$concatend", concat_count), NULL, NULL);
            push_list("LABEL", str_num("$concat", concat_count), NULL, NULL);
            if(concatTmpVar==0){
                push_list("DEFVAR", "GF@concatTmpVar", NULL, NULL);
                concatTmpVar=1;                
            }
            push_list("POPS","GF@concatTmpVar",NULL,NULL);
            push_list("POPS","GF@concatTmpVar",NULL,NULL);
            push_list("CONCAT", "GF@concatTmpVar", get_name(operand2), get_name(operand1));
            push_list("PUSHS", "GF@concatTmpVar", NULL, NULL);
            push_list("LABEL", str_num("$concatend", concat_count), NULL, NULL);

            concat_count++;
        }

        break;
    case token_minus:
        push_list("SUBS", NULL, NULL,NULL);
        break;
    case token_mul:
        push_list("MULS", NULL, NULL,NULL);
        break;
    case token_div:
        push_list("JUMPIFEQ", str_num("$divs", div_count), "GF@$type2", "string@float");
        push_list("JUMPIFEQ", "$error9", get_name(operand1), "int@0");

        push_list("IDIVS", NULL, NULL,NULL);
        push_list("JUMP", str_num("$done", div_count), NULL, NULL);
        push_list("LABEL", str_num("$divs", div_count), NULL, NULL);

        push_list("JUMPIFEQ", "$error9", get_name(operand1), "float@0x0.000000p+0");
        push_list("DIVS", NULL, NULL,NULL);
        push_list("LABEL", str_num("$done", div_count), NULL,NULL);
        div_count++;
        break;
    case token_div_div:
        if(operand1.type != token_val_int || operand2.type != token_val_int){
            fprintf(stderr, "2. bad_type error type: %d\n", 4);
            exit(4);
        }
        push_list("JUMPIFEQ", "$error9", get_name(operand2), "int@0");
        push_list("IDIVS", NULL, NULL,NULL);
        push_list("JUMP", str_num("$done", div_count), NULL, NULL);
        push_list("LABEL", str_num("$done", div_count), NULL,NULL);
        div_count++;
        break;

    case token_less_equal:
        push_list("GTS", NULL, NULL,NULL);
        push_list("NOTS", NULL, NULL,NULL);
        break;
    case token_less:

        push_list("LTS", NULL, NULL,NULL);
        break;
    case token_great_equal:
        push_list("LTS", NULL, NULL,NULL);
        push_list("NOTS", NULL, NULL,NULL);
        break;
    case token_great:
        push_list("GTS", NULL, NULL,NULL);
        break;
    case token_neg_equal:
        push_list("EQS", NULL, NULL,NULL);
        push_list("NOTS", NULL, NULL,NULL);
        break;
    case token_equal_equal:
        push_list("EQS", NULL, NULL,NULL);
        break;
    default:
        break;
    }
}
//change order of expression elements
void infix_postfix(char* act_func, char* id){
    stack_t* output_stack = S_Init();
    stack_t* operator_stack = S_Init();
    int num_of_parenth = 0;
    int sum_count = 1;
    bool logic = false;
    Token_t tmp;

    if(id == NULL){
        if(token->type != token_left_bracket){
            tmp = *token;
            get_next_token(f, token);
            if(token->type == token_equal){
                id = tmp.val.c;
                get_next_token(f, token);
            } else {
                if(tmp.type == token_id){
                    if(!stl_search(tabulka, tmp.val.c, act_func)){
                            fprintf(stderr, "1. infix_postfix error type: %d\n", 3);
                            exit(3);
                            return;
                    }
                }
                sum_count--;
                S_Push_Token(output_stack, tmp);
            }
        }else{
            fprintf(stderr,"565: infix_postfix err type 2\n");
            exit(2);
        }


    }

    while(token->type != token_eol && token->type != token_colon && token->type != token_dedent && token->type != token_right_bracket){
        if(is_operand()){
            if(token->type == token_id){
                if(!stl_search(tabulka, token->val.c, act_func)){
                    get_next_token(f, token);
                    if(token->type == token_left_bracket){
                        get_next_token(f, token);
                        if(token->type == token_right_bracket){
                            fprintf(stderr, "1. undefined_function error type : %d\n", 3);
                            exit(3);
                        }   else    {
                            fprintf(stderr, "1. bad_call error type : %d\n", 2);
                            exit(2);
                        }
                    }else{
                        
                        fprintf(stderr, "2. infix_postfix error type: %d\n", 3);
                        exit(3);
                    }
                    return;
                }
            }
            sum_count--;
            S_Push_Token(output_stack, *token);
        }else if(is_arithmetic_operator() || is_logic_operator()){
            if(is_logic_operator()){
                logic = true;
            }
            sum_count++;
            oper(output_stack, operator_stack);
        }else if(token->type == token_left_bracket){
            num_of_parenth++;
            S_Push_Token(operator_stack, *token);
        }else if(token->type == token_right_bracket){
            num_of_parenth--;
            righ_parenth(output_stack, operator_stack);
        }else {
            fprintf(stderr, "3. infix_postfix error type: %d\n", 2);
            exit(2);
            return;
        }
        get_next_token(f, token);
    }
    while(!S_Empty(operator_stack)){
        Token_t tmp = S_Top_token(operator_stack);
        S_Pop(operator_stack);
        S_Push_Token(output_stack, tmp);
    }
    if(sum_count !=0 || num_of_parenth != 0){
        fprintf(stderr, "4. infix_postfix error type: %d\n", 2);
        exit(2);
        return;
    }

    postfix_instruction(output_stack, act_func, logic);
    if(id!= NULL && logic){
        fprintf(stderr, "5. infix_postfix error type: %d\n", 4);
        exit(4);
        return;
    }
    if(id != NULL){
        push_list("POPS", concat("LF@", id), NULL, NULL);
        if(strcmp(act_func, "Main")){
            push_list("MOVE", "LF@%retval", concat("LF@", id), NULL);
        }
    }else {
        if(!logic){
            push_list("POPS", "LF@%retval", NULL, NULL);
        }
    }
    S_free(output_stack);
    S_free(operator_stack);

}
//evaluating expression
void expression(char* act_func, char* id){
    if(is_builtin_f()){
        builtin_f(act_func, id);
        return;
    }else if(token->type == token_id){
        if(stl_find_func(tabulka, token->val.c)){
            user_f(act_func, id);
            return;
        }
    }
    infix_postfix(act_func, id);
}
