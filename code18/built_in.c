#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "built_in.h"

//uprava stringu pro print v IFJcode18
char* string_ready(char* string){
    int string_size = strlen(string);
    char* res_string=malloc(sizeof(string)*1000);
    char* tmp_string=malloc(sizeof(string)*1000);
    for(int i=0;i<string_size;i++){
        if((((int)string[i])>=0 && ((int)string[i])<33) || ((int)string[i])==35 || ((int)string[i])==92 ){

            sprintf(res_string,"%s%c",res_string,(int)string[i]);
        }else{
            sprintf(res_string,"%s%c",res_string,string[i]);
        }
    }
    free(tmp_string);
    return res_string;
}

void builtin_print(char* act_f){
        bool zavorky = false;
        if(token->type == token_left_bracket){
            zavorky=true;
            get_next_token(f,token);
        }
        char *str;
        if((str=malloc(sizeof(float)*sizeof(char*)*20))==NULL){
            printf("Memory error\n");
        }
        char *print_string;
        if((print_string=malloc(sizeof(char*)))==NULL){
            printf("Memory error\n");
        }
        static bool printvar=false;
        if(printvar==false){
            push_list("DEFVAR","GF@printvar",NULL,NULL);
            printvar=true;
        }
        while(token->type == token_string || token->type == token_val_int || token->type == token_val_float || token->type == token_id){
            switch(token->type){
                case(39)://string
                    print_string=realloc(print_string,sizeof(char)*strlen(token->val.c)*2);
                    print_string=string_ready(token->val.c);
                    str=realloc(str,sizeof(char)*(strlen(token->val.c)+7));
                    sprintf(str,"string@%s",print_string);
                    push_list("WRITE", str,NULL,NULL);
                    break;
                case(37)://int
                    sprintf(str,"int@%d",token->val.i);
                    push_list("WRITE", str,NULL,NULL);
                    break;
                case(38)://float
                    sprintf(str,"float@%a",token->val.d);
                    push_list("WRITE", str,NULL,NULL);
                    break;
                case(36)://Promenna
                    if(stl_search(tabulka, token->val.c, act_f)){
                        sprintf(str,"LF@%s",token->val.c);
                        push_list("WRITE", str,NULL,NULL);
                    } else {
                        fprintf(stderr,"error type:%d\n", 3);
                        exit(3);
                    }
                    break;
                default:
                    fprintf(stderr, "error type:%d\n", 2);
                    exit(2);
                }

            get_next_token(f,token);
            if(token->type==token_comma){
                ;//toto nemazat, ma tu byt samostatnej strednik
            }else if(zavorky==true){
                    if(token->type==token_right_bracket){
//                        free(str);
//                        free(print_string);
                        get_next_token(f, token);
                        if(token->type == token_eol){
                            break;
                        }else{
                            fprintf(stderr, "error type: %d", 2);
                            exit(2);
                        }
                    }else{
//                        free(str);
//                        free(print_string);
                        fprintf(stderr, "error type: %d", 2);
                        exit(2);
                        //error print nekonci zavorkou a mel by
                    }
            }else if(token->type==token_eol){
                //konec print prikazu
//                free(str);
//                free(print_string);
                break;
            }else if(token->type==token_eof){
//                free(str);
//                free(print_string);
                break;
            }else if (token->type==token_print){
//                free(str);
//                free(print_string);
                break;
            }else{
                fprintf(stderr, "error type:%d\n", 2);
//                free(str);
//                free(print_string);
                exit(2);
                return;
            }
            get_next_token(f,token);
        }

}

void builtin_inputi(char* res_var){
    char str[100];
    sprintf(str,"LF@%s",res_var);
    push_list("READ", str, "int", NULL);
}
void builtin_inputf(char* res_var){
    char str[100];
    sprintf(str,"LF@%s",res_var);
    push_list("READ", str, "float", NULL);
}
void builtin_inputs(char* res_var){
    char str[100];
    sprintf(str,"LF@%s",res_var);
    push_list("READ",str,"string",NULL);
}

void builtin_length(char* id, char* act_func){
//    get_next_token(f, token);
    bool zavorky = false;
    if(token->type == token_left_bracket){
        zavorky=true;
        get_next_token(f,token);
    }
    char str[100];
    if(token->type == token_id || token->type == token_string){
        if(token->type == token_string){
            sprintf(str,"LF@%s", id);
            push_list("STRLEN", str, concat("string@", token->val.c), NULL);
        } else {
            if(!stl_search(tabulka, token->val.c, act_func)){
                fprintf(stderr, "error type:%d\n", 3);
                exit(3);
            }
            char tmp [100];
            sprintf(str,"LF@%s", token->val.c);
            push_list("TYPE","GF@type1", str, NULL);
            push_list("JUMPIFNEQ","$error4", "GF@type1", "string@string");
            if(id != NULL){
                sprintf(tmp, "LF@%s", id);
                push_list("STRLEN", tmp, str ,NULL);
                push_list("PUSHS", tmp, NULL, NULL);
            } else {
                push_list("STRLEN", "GF@type2", str ,NULL);
                push_list("PUSHS", "GF@type2", NULL, NULL);
            }
        }
    }
    get_next_token(f, token);
    if(zavorky){
        if(token->type == token_right_bracket){
            get_next_token(f, token);
        } else {
            fprintf(stderr, "error type: %d", 2);
            exit(2);
        }
    }
    if(token->type == token_eol){
        return;
    }else{
        fprintf(stderr, "error type: %d", 2);
        exit(2);
    }
}

void builtin_chr_generate(){
    static bool len_exist=false;

    if(len_exist==false){
    push_list("LABEL","chr",NULL,NULL);
    push_list("PUSHFRAME",NULL,NULL,NULL);
    push_list("DEFVAR", "LF@retval", NULL, NULL);
    push_list("DEFVAR","LF@cmpres",NULL,NULL);
    push_list("DEFVAR","LF@i",NULL,NULL);
    push_list("POPS","LF@i",NULL,NULL);
    push_list("DEFVAR","LF@var_type",NULL,NULL);
    push_list("TYPE","LF@var_type","LF@i",NULL);
    push_list("JUMPIFEQ","chr_ijeint","LF@var_type","string@int");
    push_list("EXIT","int@4",NULL,NULL);
    push_list("LABEL","chr_ijeint",NULL,NULL);
    push_list("LT","LF@cmpres","LF@i","int@0");
    push_list("JUMPIFNEQ","chr_iGEQ0","bool@true","LF@cmpres");
    push_list("EXIT","int@1",NULL,NULL);
    push_list("LABEL","chr_iGEQ0",NULL,NULL);
    push_list("GT","LF@cmpres","LF@i","int@255");
    push_list("JUMPIFNEQ","chr_igood","bool@true","lf@cmpres");
    push_list("EXIT","int@1",NULL,NULL);
    push_list("LABEL","chr_igood",NULL,NULL);
    push_list("INT2CHAR","LF@retval","lf@i",NULL);
    push_list("POPFRAME",NULL,NULL,NULL);
    push_list("RETURN",NULL,NULL,NULL);
    len_exist=true;
    }
}

void builtin_chr_call(SymbolTable_t *fce_list,char* res_var,char* actual_fce){
    get_next_token(f,token);
    if(token->type!=token_left_bracket){
        fprintf(stderr,"%d\n",1);//syntax error
        return;
    }
    get_next_token(f,token);

    if(token->type!=token_val_int && token->type!=token_id){
        fprintf(stderr,"%d\n",4);//spatna promena
        exit(4);
    }
    char* param_string=malloc(sizeof(char)*10);
    if(token->type==token_val_int){
        sprintf(param_string,"int@%d",token->val.i);
    }else if(token->type==token_id){
        sprintf(param_string,"LF@%s",token->val.c);
    }

    char* res_var_frame=malloc(sizeof(char)*strlen(res_var)+3);
    sprintf(res_var_frame,"LF@%s",res_var);

    if(stl_search(fce_list,res_var,actual_fce)==false){
        //create var
        stl_insert_to_top(tabulka,res_var,actual_fce);
        push_list("DEFVAR",res_var_frame,NULL,NULL);

    }
    push_list("CREATEFRAME",NULL,NULL,NULL);
    push_list("PUSHS",param_string,NULL,NULL);
    push_list("CALL","chr",NULL,NULL);
    push_list("MOVE",res_var_frame,"TF@retval",NULL);
    free(res_var_frame);
}

void builtin_ord_generate(){
    static bool len_exist=false;
    if(len_exist==false){
    push_list("LABEL","ord",NULL,NULL);
    push_list("PUSHFRAME",NULL,NULL,NULL);
    push_list("DEFVAR","LF@retval",NULL,NULL);
    push_list("DEFVAR","LF@cmpres",NULL,NULL);
    push_list("DEFVAR","LF@i",NULL,NULL);
    push_list("DEFVAR","LF@s",NULL,NULL);
    push_list("POPS","LF@i",NULL,NULL);
    push_list("POPS","LF@s",NULL,NULL);
    push_list("DEFVAR","LF@var_type",NULL,NULL);
    push_list("TYPE","LF@var_type","LF@i",NULL);
    push_list("JUMPIFNEQ","ord_err","LF@var_type","string@int");
    push_list("TYPE","LF@var_type","LF@s",NULL);
    push_list("JUMPIFNEQ","ord_err","LF@var_type","string@string");
    push_list("LT","LF@cmpres","LF@i","int@0");
    push_list("JUMPIFNEQ","ord_iGEQ0","bool@true","LF@cmpres");
    push_list("MOVE","LF@retval","nil@nil",NULL);
    push_list("POPFRAME",NULL,NULL,NULL);
    push_list("RETURN",NULL,NULL,NULL);
    push_list("LABEL","ord_iGEQ0",NULL,NULL);
    push_list("DEFVAR","LF@strlen",NULL,NULL);
    push_list("STRLEN","LF@strlen","LF@s",NULL);
    push_list("MOVE","LF@retval","LF@strlen",NULL);
    push_list("SUB","LF@strlen","LF@strlen","int@1");
    push_list("GT","LF@cmpres","LF@i","LF@strlen");
    push_list("JUMPIFNEQ","ord_igood","bool@true","lf@cmpres");
    push_list("MOVE","LF@retval","nil@nil",NULL);
    push_list("POPFRAME",NULL,NULL,NULL);
    push_list("RETURN",NULL,NULL,NULL);
    push_list("LABEL","ord_igood",NULL,NULL);
    push_list("STRI2INT","LF@retval","LF@s","LF@i");
    push_list("POPFRAME",NULL,NULL,NULL);
    push_list("RETURN",NULL,NULL,NULL);
    push_list("LABEL","ord_err",NULL,NULL);
    push_list("EXIT","int@4",NULL,NULL);

    len_exist=true;
    }
}

void builtin_ord_call(SymbolTable_t *f_list,char* res_var,char* actual_fce){

    get_next_token(f,token);
    if(token->type!=token_left_bracket){//parametry musi zacinat zavorkou
        fprintf(stderr,"%d\n",1);//syntax error
        exit(1);
    }
    get_next_token(f,token);//1.parametr

    if(token->type!=token_string && token->type!=token_id){
        fprintf(stderr,"%d\n",4);//spatna promena
        exit(4);
    }

    char* param1_string=malloc(sizeof(char)*strlen(token->val.c));
    if(token->type==token_string){
        sprintf(param1_string,"string@%s",token->val.c);
    }else if(token->type==token_id){
        sprintf(param1_string,"LF@%s",token->val.c);
    }

    get_next_token(f,token);
    if(token->type!=token_comma){//mezi parametrama musi byt carka
        fprintf(stderr,"%d\n",1);//syntax error
        exit(1);
    }
    get_next_token(f,token);//2. parametr
    if(token->type!=token_val_int && token->type!=token_id){
        fprintf(stderr,"%d\n",4);//spatna promena
        exit(4);
    }

    char* param2_string=malloc(sizeof(char)*100);
    if(token->type==token_val_int){
        sprintf(param2_string,"int@%d",token->val.i);
    }else if(token->type==token_id){
        sprintf(param2_string,"LF@%s",token->val.c);
    }


    char* res_var_frame=malloc(sizeof(char)*strlen(res_var)+3);
    sprintf(res_var_frame,"LF@%s",res_var);

    if(stl_search(f_list,res_var,actual_fce)==false){
        //create var
        stl_insert_to_top(f_list,res_var,actual_fce);
        push_list("DEFVAR",res_var_frame,NULL,NULL);

    }
    push_list("CREATEFRAME",NULL,NULL,NULL);
    push_list("PUSHS",param1_string,NULL,NULL);
    push_list("PUSHS",param2_string,NULL,NULL);
    push_list("CALL","ord",NULL,NULL);
    push_list("MOVE",res_var_frame,"TF@retval",NULL);
    free(res_var_frame);
}

void builtin_substr_generate(){
    static bool len_exist=false;
    if(len_exist==false){
    push_list("LABEL","substr",NULL,NULL);
    push_list("PUSHFRAME",NULL,NULL,NULL);

    push_list("DEFVAR","LF@retval",NULL,NULL);
    push_list("DEFVAR","LF@cmpres",NULL,NULL);

    push_list("DEFVAR","LF@n",NULL,NULL);
    push_list("DEFVAR","LF@i",NULL,NULL);
    push_list("DEFVAR","LF@s",NULL,NULL);
    push_list("POPS","LF@n",NULL,NULL);
    push_list("POPS","LF@i",NULL,NULL);
    push_list("POPS","LF@s",NULL,NULL);

    push_list("DEFVAR","LF@var_type",NULL,NULL);
    push_list("TYPE","LF@var_type","LF@n",NULL);
    push_list("JUMPIFNEQ","substr_err","LF@var_type","string@int");
    push_list("TYPE","LF@var_type","LF@i",NULL);
    push_list("JUMPIFNEQ","substr_err","LF@var_type","string@int");
    push_list("TYPE","LF@var_type","LF@s",NULL);
    push_list("JUMPIFNEQ","substr_err","LF@var_type","string@string");

    push_list("JUMPIFEQ","substr_str0","LF@s","string@");
    push_list("LT","LF@cmpres","LF@i","int@0");
    push_list("JUMPIFNEQ","substr_iGEQ0","LF@cmpres","bool@true");
    push_list("MOVE","LF@retval","nil@nil",NULL);
    push_list("POPFRAME",NULL,NULL,NULL);
    push_list("RETURN",NULL,NULL,NULL);

    push_list("LABEL","substr_iGEQ0",NULL,NULL);
    push_list("DEFVAR","LF@strlen",NULL,NULL);
    push_list("STRLEN","LF@strlen","LF@s",NULL);
    push_list("GT","LF@cmpres","LF@i","LF@strlen");
    push_list("JUMPIFNEQ","substr_igood","bool@true","LF@cmpres");
    push_list("MOVE","LF@retval","nil@nil",NULL);
    push_list("POPFRAME",NULL,NULL,NULL);
    push_list("RETURN",NULL,NULL,NULL);

    push_list("LABEL","substr_igood",NULL,NULL);
    push_list("GT","LF@cmpres","LF@n","int@0");
    push_list("JUMPIFEQ","substr_ngood","bool@true","LF@cmpres");
    push_list("MOVE","LF@retval","nil@nil",NULL);
    push_list("POPFRAME",NULL,NULL,NULL);
    push_list("RETURN",NULL,NULL,NULL);

    push_list("LABEL","substr_ngood",NULL,NULL);
    push_list("ADD","LF@cmpres","LF@i","LF@n");
    push_list("GT","LF@cmpres","LF@cmpres","LF@strlen");
    push_list("JUMPIFNEQ","substr_nrdy","bool@true","lf@cmpres");
    push_list("SUB","LF@n","LF@strlen","LF@i");
    push_list("LABEL","substr_nrdy",NULL,NULL);
    push_list("DEFVAR","LF@char",NULL,NULL);
    push_list("MOVE","LF@retval","string@",NULL);

    push_list("LABEL","substr_while",NULL,NULL);
    push_list("JUMPIFEQ","substr_konec","lf@n","int@0");
    push_list("GETCHAR","LF@char","LF@s","LF@i");
    push_list("CONCAT","LF@retval","LF@retval","LF@char");
    push_list("SUB","LF@n","LF@n","int@1");
    push_list("ADD","LF@i","LF@i","int@1");
    push_list("JUMP","substr_while",NULL,NULL);

    push_list("LABEL","substr_konec",NULL,NULL);
    push_list("POPFRAME",NULL,NULL,NULL);
    push_list("RETURN",NULL,NULL,NULL);

    push_list("LABEL","substr_err",NULL,NULL);
    push_list("EXIT","int@4",NULL,NULL);

    len_exist=true;
    }
}

void builtin_substr_call(SymbolTable_t *f_list,char* res_var,char* actual_fce){
//    get_next_token(f,token);
    bool zavorky = false;
    if(token->type == token_left_bracket){
        zavorky = true;
        get_next_token(f,token);
    }
//    if(token->type != ){//parametry musi zacinat zavorkou
//        fprintf(stderr,"error type: %d\n", 2);//syntax error
//        exit(2);
//    }
//    get_next_token(f,token);//1.parametr (string)

    if(token->type!=token_string && token->type!=token_id){
        fprintf(stderr,"error type: %d\n",4);//spatna promena
        exit(4);
    }
    char* param1_string=malloc(sizeof(char)*strlen(token->val.c));
    if(token->type==token_string){
        sprintf(param1_string,"string@%s",token->val.c);
    }else if(token->type==token_id){
        sprintf(param1_string,"LF@%s",token->val.c);
    }

    get_next_token(f,token);//mela by to byt carka
    if(token->type!=token_comma){
        fprintf(stderr,"%d\n",1);//syntax error
        exit(1);
    }

    get_next_token(f,token);//2. parametr
    if(token->type!=token_val_int && token->type!=token_id){
        fprintf(stderr,"%d\n",4);//spatna promena
        exit(4);
    }

    char* param2_string=malloc(sizeof(char)*100);
    if(token->type==token_val_int){
        sprintf(param2_string,"int@%d",token->val.i);
    }else if(token->type==token_id){
        sprintf(param2_string,"LF@%s",token->val.c);
    }

    get_next_token(f,token);
    if(token->type!=token_comma){//mezi parametrama musi byt carka
        fprintf(stderr,"%d\n",1);//syntax error
        exit(1);
    }
    get_next_token(f,token);//3. parametr

    if(token->type!=token_val_int && token->type!=token_id){
        fprintf(stderr,"%d\n",4);//spatna promena
        exit(4);
    }
    char* param3_string=malloc(sizeof(char)*100);
    if(token->type==token_val_int){
        sprintf(param3_string,"int@%d",token->val.i);
    }else if(token->type==token_id){
        sprintf(param3_string,"LF@%s",token->val.c);
    }

    char* res_var_frame=malloc(sizeof(char)*(strlen(res_var)+3));
    sprintf(res_var_frame,"LF@%s",res_var);

    if(stl_search(f_list,res_var,actual_fce)==false){
        //create var
        stl_insert_to_top(f_list,res_var,actual_fce);
        push_list("DEFVAR",res_var_frame,NULL,NULL);

    }
    push_list("CREATEFRAME",NULL,NULL,NULL);
    push_list("PUSHS",param1_string,NULL,NULL);
    push_list("PUSHS",param2_string,NULL,NULL);
    push_list("PUSHS",param3_string,NULL,NULL);
    push_list("CALL","substr",NULL,NULL);
    push_list("MOVE",res_var_frame,"TF@retval",NULL);
    free(res_var_frame);
    get_next_token(f, token);
    if(zavorky){
        if(token->type == token_right_bracket){
            get_next_token(f, token);
        } else{
            fprintf(stderr,"error type: %d", 2);
            exit(2);
        }
    }

}
