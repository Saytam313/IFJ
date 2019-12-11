#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "built_in.h"

char* current_function = "Main";
char* current_variable;

int in_if=0;//0- nothing | 1- if | 2- else
char in_if_vars[100][1000];
int last_in_if_var=0;
bool PROG();
bool DEF ();
bool STATEMENT();
bool STATEMENT_LIST();
bool NEXT_DEF_PARAM();
bool DEF_PARAM_LIST();
char* get_label_name(int n, char c);
char* get_param_name();

//concatenates 2 strings
char* concat(char* str1, char* str2){
    char* vysledek = (char *)malloc(strlen(str1) + strlen(str2) + 1);
    sprintf(vysledek, "%s%s", str1, str2);
    return vysledek;
}

//returns name of var in IFJcode19 form
char* get_var_name(char* name){
    char* vysledek = (char *)malloc(strlen("LF@")+strlen(name));
    sprintf(vysledek, "LF@%s", name);
    return vysledek;
}
//returns IFJcode19 for param
char* get_param_name(){
    int delka = (int)(log10(tabulka->param_count)+1);
    char* vysledek = (char *)malloc(delka * sizeof(char) + 5);
    sprintf(vysledek, "LF@%%%d", tabulka->param_count);
    return vysledek;
}
//generates new label name
char * get_label_name(int n, char c){
    int delka = (int)(log10(n))+1;
    char* vysledek = (char *)malloc(delka * sizeof(char) + 3);
    vysledek [0] = '$';
    vysledek [1] = c;
    sprintf(vysledek+2, "%d", n);
    return vysledek;
}
//generates individual params for user functions
bool NEXT_DEF_PARAM(){
    get_next_token(f, token);
    if(token->type == token_comma){
        get_next_token(f, token);
        if(token->type == token_id){
            stl_insert_top(tabulka, token->val.c);
            tabulka->param_count++;
            push_list ("DEFVAR", get_var_name(token->val.c), NULL, NULL);
            push_list("MOVE", get_var_name(token->val.c), get_param_name(), NULL);
            return NEXT_DEF_PARAM();
        }
    } else if (token->type == token_right_bracket){
        return true;
    }
    fprintf(stderr, "NEXT_DEF_PARAM error type: %d\n", 2);
    exit(2);
    return 0;
}
//reads params when defining user function
bool DEF_PARAM_LIST(){
    get_next_token(f, token);
    if(token->type == token_right_bracket){
        return true;
    } else if(token->type == token_id){
        stl_insert_top(tabulka, token->val.c);
        tabulka->param_count++;
        push_list ("DEFVAR", get_var_name(token->val.c), NULL, NULL);
        push_list("MOVE", get_var_name(token->val.c), get_param_name(), NULL);
        return NEXT_DEF_PARAM();
    }
    fprintf(stderr, "DEF_PARAM_LIST error type: %d\n", 2);
    exit(2);
    return 0;
}

//generates code for user functions
bool DEF (){
    get_next_token(f, token);
    if(token->type == token_id){
        if(stl_find_func(tabulka, token->val.c)){
            fprintf(stderr, "1 DEF error type: %d\n", 3);
            exit(3);
        }
        stl_push(&tabulka, token->val.c);
        current_function = token->val.c;

        push_list("JUMP", get_label_name(stl_number_of_func(tabulka), 'm'), NULL, NULL);
        push_list ("LABEL", token->val.c, NULL, NULL);
        push_list ("PUSHFRAME", NULL, NULL, NULL);
        push_list ("DEFVAR", "LF@%retval", NULL, NULL);
        push_list ("MOVE", "LF@%retval", "nil@nil", NULL);

        get_next_token(f, token);
        if(token->type == token_left_bracket){
            if(DEF_PARAM_LIST()){
                get_next_token(f, token);
                if(token->type == token_colon){
                    get_next_token(f, token);
                    if(token->type == token_eol){
                        get_next_token(f, token);
                        if(token->type == token_indent){
                            get_next_token(f, token);
                            if(STATEMENT_LIST()){
                                if(last_token.type == token_dedent){
                                    current_function = "Main";
                                    push_list ("POPFRAME", NULL, NULL, NULL);
                                    push_list ("RETURN", NULL, NULL, NULL);
                                    push_list("LABEL", get_label_name(stl_number_of_func(tabulka), 'm'), NULL, NULL);
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fprintf(stderr, "2 DEF error type: %d\n", 2);
    exit(2);
    return 0;
}
//reads body of if, while and defined functions
bool STATEMENT_LIST(){
    while(token->type == token_eol || token->type == token_pass){
        get_next_token(f, token);
    }
    if(token->type == token_id ||  token->type == token_print  || token->type == token_inputf
        ||  token->type == token_inputi  ||  token->type == token_inputs   ||  token->type == token_length  ||  token->type == token_substr  ||
        token->type == token_ord  ||  token->type == token_chr  || token->type == token_if || token->type == token_while ||
        token->type == token_val_float || token->type == token_val_int || token->type == token_string || token->type == token_pass || token->type == token_return){
        return STATEMENT()&& STATEMENT_LIST();
    } else if(token->type == token_else || token->type == token_dedent){
        last_token = *token;
        get_next_token(f, token);
        return true;
    } else {
    return false;
    }
}
//evaluates statement
bool STATEMENT(){
    bool zavorky=false;
    if(token->type == token_eol || token->type == token_pass){
        return true;
    } else if(((token->type >= token_inputs) && (token->type <= token_substr)) || token->type == token_chr){
        expression(current_function, NULL);
        return true;
    } else if(token->type == token_id){
        current_variable = token->val.c;
        if(stl_find_func(tabulka, token->val.c)){
            expression(current_function, NULL);
            return true;
        } else if(!stl_search(tabulka, current_variable, current_function)){
            get_next_token(f, token);
            if(token->type == token_equal){
                if(in_if==1){
                    strcpy(in_if_vars[last_in_if_var],get_var_name(current_variable));
                    last_in_if_var++;
                }
                stl_insert_to_top(tabulka, current_variable, current_function);
                push_list ("DEFVAR", get_var_name(current_variable), NULL, NULL);
                get_next_token(f, token);
                expression(current_function, current_variable);
                return 1;
            }else {
                fprintf(stderr, "1 STATEMENT error type: %d\n", 2);
                exit(2);
                return 0;
            }
        }else {

            last_token = *token;
            get_next_token(f, token);
            if(token->type == token_equal){
                if(in_if==2){
                    for(int i=0;last_in_if_var>i;i++){
                        if(strcmp(in_if_vars[i],get_var_name(current_variable))==0){
                            push_list ("DEFVAR", get_var_name(current_variable), NULL, NULL);
                            strcpy(in_if_vars[i],"");
                        }

                    }

                }


                get_next_token(f, token);
                expression(current_function, current_variable);
            }else{
                push_list("PUSHS", concat("LF@", last_token.val.c), NULL, NULL);
                return true;
            }
        }
        return true;
    } else if(token->type == token_if){
        in_if=1;
        zavorky=false;
        get_next_token(f, token);
        if(token->type == token_colon || token->type == token_eol ){
            fprintf(stderr, "error 2 %d\n", 2);
            exit(2);
        }
        if(token->type == token_left_bracket){
            zavorky=true;
            get_next_token(f, token);
        }
        expression(current_function, NULL);
        static int ifcounter = 0;
        ifcounter++;
        char* elselabel = get_label_name(ifcounter, 'l');
        char* endlabel = get_label_name(ifcounter, 'n');
        S_Push(label_Stack, endlabel);
        S_Push(label_Stack, elselabel);
        push_list ("POPS", "GF@$result", NULL, NULL);
        push_list("NOT", "GF@$result", "GF@$result", NULL);
        push_list("JUMPIFEQ", elselabel , "GF@$result", "bool@true");
        if(zavorky==true && token->type != token_right_bracket){
            fprintf(stderr, "if bracket missing error 2\n");
            exit(2);
        }else if(zavorky==true){
            get_next_token(f, token);
            zavorky=false;
        }

        if(token->type == token_colon){
            get_next_token(f, token);
            if(token->type == token_eol){
                get_next_token(f, token);
                if(token->type == token_indent){
                    get_next_token(f, token);
                    if(STATEMENT_LIST()){

                        elselabel = S_Top(label_Stack);
                        S_Pop(label_Stack);
                        push_list("JUMP", endlabel, NULL, NULL);
                        push_list("LABEL", elselabel, NULL, NULL);
                        if(last_token.type == token_dedent){
                            if(token->type == token_else){
                                in_if=2;
                                get_next_token(f, token);
                                if(token->type == token_colon){
                                    get_next_token(f, token);
                                    if(token->type== token_eol){
                                        get_next_token(f, token);
                                        if(token->type == token_indent){
                                            get_next_token(f, token);
                                            if(STATEMENT_LIST()){
                                                endlabel = S_Top(label_Stack);
                                                S_Pop(label_Stack);
                                                push_list("LABEL", endlabel, NULL, NULL);
                                                if(last_token.type == token_dedent){
                                                    in_if=0;
                                                    for(int i=0;last_in_if_var>i;i++){
                                                        strcpy(in_if_vars[i],"");
                                                    }
                                                    last_in_if_var=0;
                                                    return true;
                                                }
                                        }
                                        }
                                    }
                                }
                            }else{
                                endlabel = S_Top(label_Stack);
                                S_Pop(label_Stack);
                                push_list("LABEL", endlabel, NULL, NULL);
                                in_if=0;
                                for(int i=0;last_in_if_var>i;i++){
                                    strcpy(in_if_vars[i],"");
                                }
                                last_in_if_var=0;
                                return true;    
                            }
                        }
                    }
                }
            }
        }

    } else if(token->type == token_while){
        get_next_token(f, token);
        zavorky=false;
        static int whilecounter = 0;
        whilecounter++;
        if(token->type == token_left_bracket){
            zavorky=true;
            get_next_token(f, token);
        }
        char * whilelabel = get_label_name(whilecounter, 'w');
        push_list("LABEL", whilelabel, NULL, NULL);
        char * whilendlabel = get_label_name(whilecounter, 'e');
        S_Push(label_Stack, whilendlabel);
        S_Push(label_Stack, whilelabel);
        expression(current_function, NULL);
        push_list ("POPS", "GF@$result", NULL, NULL);
        push_list("NOT", "GF@$result", "GF@$result", NULL);
        push_list("JUMPIFEQ", whilendlabel , "GF@$result", "bool@true");
        
        if(zavorky==true && token->type != token_right_bracket){
            fprintf(stderr, "if bracket missing error 2\n");
            exit(2);
        }else if(zavorky==true){
            get_next_token(f, token);
            zavorky=false;
        }


        if(token->type == token_colon){
            get_next_token(f, token);
            if(token->type == token_eol){
                get_next_token(f,token);
                if(token->type == token_indent){
                    get_next_token(f, token);
                    if(STATEMENT_LIST()){
                        whilelabel = S_Top(label_Stack);
                        S_Pop(label_Stack);
                        whilendlabel = S_Top(label_Stack);
                        S_Pop(label_Stack);
                        push_list("JUMP", whilelabel, NULL, NULL);
                        push_list("LABEL", whilendlabel, NULL, NULL);
                        if(last_token.type == token_dedent){
                            return true;
                        }
                    }
                }
            }
        }
    } else if(token->type >= token_val_int && token->type <= token_string){
        expression(current_function, NULL);
        return true;
    } else if(token->type == token_return){
        if(strcmp(current_function,"Main")!=0){
            get_next_token(f, token);
            expression(current_function,"%retval");
            return true;


        }
    }
    fprintf(stderr, "2 STATEMENT error type: %d\n", 2);
    exit(2);
    return 0;

}
//reads main body of program
bool PROG(){
    if(last_token.type != token_dedent){
        get_next_token(f, token);
    }else{
        if(token->type==token_dedent){
            get_next_token(f, token);
        }
        last_token = *token;
    }
    if(token->type == token_def){
        if(DEF()){
            if(token->type == token_eol || last_token.type == token_dedent){
                return PROG();
            }
        }
    } else if(token->type == token_eol || token->type == token_nic || token->type == token_pass || token->type == token_doc_string ) {
        return PROG();
    }else if(token->type == token_eof){
        return 1;
    } else {
        if(STATEMENT() == true){
            if(token->type == token_eol || last_token.type == token_dedent || token->type == token_dedent){
                return PROG();
            }
        }
    }
    fprintf(stderr, "PROG error type: %d\n", 2);
    exit(2);
    return 0;
}
//start of code parsing
void parse( ){
    label_Stack = S_Init();
    stl_init(&tabulka);
    token = create_new_token();
    if(scanner_init()==0){
        return;
    }

    f = stdin;

    push_list("CREATEFRAME", NULL, NULL, NULL);
    push_list("PUSHFRAME", NULL, NULL, NULL);
    push_list("DEFVAR", "GF@$type1", NULL, NULL);
    push_list("DEFVAR", "GF@$type2", NULL, NULL);
    push_list("DEFVAR", "GF@$result", NULL, NULL);
    push_list("MOVE", "GF@$result", "nil@nil", NULL);

    PROG();

    push_list("JUMP", "$endend", NULL, NULL);
    builtin_chr_generate(0);
    builtin_ord_generate(0);
    builtin_substr_generate(0);
    push_list("LABEL", "$error4", NULL, NULL);
    push_list("EXIT", "int@4", NULL, NULL);
    push_list("LABEL", "$error9", NULL, NULL);
    push_list("EXIT", "int@9", NULL, NULL);
    push_list("LABEL", "$endend", NULL, NULL);
    S_free(label_Stack);
    scanner_free();
}

