/*
*           Škola: Brno University of Technology, Faculty of Information Technology
*  Název projektu: IFJ19
*        Předměty: IFJ a IAL
*            Část: Scanner
*             Tým: 015
*        Varianta: I
*          Autoři: Aleš Tetur          (xtetur01)
*                  Šimon Matyáš        (xmatya11)
*/

#include "scanner.h"

#define TOKEN_STRING_SIZE 255
#define BUFFER_SIZE 512
#define SUCCES 0
#define LEX_ERR 1
#define SYNT_ERR 2
#define SEM_DEF_ERR 3
#define SEM_RUN_ERR 4
#define SEM_PARAM_ERR 5
#define SEM_OTH_ERR 6
#define NUL_DIV_ERR 9


//Funkce vrací token konce vstupu při interní chybě, kdy není možné přidat další znak do proměnné st
#define return_eof_false(ptr) {     \
    (ptr)->type = token_eof;        \
    return false;                   \
}                                   \

//Struktura pro průběžné ukládání řetězců
typedef struct token_var_string {
    int length;
    int max;
    char *string;
} string_t;

// Inicializace prostoru na ukládání názvů a hodnot proměnných
bool tokenString(string_t *st) {
    assert(st);

    st->string = (char *)malloc(sizeof(char)*TOKEN_STRING_SIZE);

    if(st->string == NULL) {
        fprintf(stderr, "%s:%d\n", "ERROR", 99);
        exit(99);
    }

    st->string[0] = '\0';
    st->length = 0;
    st->max = TOKEN_STRING_SIZE - 1;
    return true;
}

//Přidání znaku do proměnné s hodnotami
bool tokenStringPridChar(string_t *st, char c) {
    assert(st);
    assert(st->string);

    if(st->length == st->max) {
        fprintf(stderr, "%s:%d\n", "ERROR", 99);
        exit(99);
    }

    st->string[st->length] = c;
    st->length++;
    st->string[st->length] = '\0';
    return true;
}


//Pomocné proměnné
static string_t val;            //Proměnná pro hodnotu
static unsigned int line;       //Proměnná s počtem načtených řádků
static unsigned int str_doc = 0;//Proměnná s počtem uvozovek pro dokum. řetězec
static unsigned int val_tab = 0;//Proměnná s počtem odřádkovani
static bool set_eof = true;     //Proměnná indikující konec souboru
static bool empty_line = true;  //Proměnná indikující prázdný řádek
static bool set_nul = false;    //Proměnná indikující zadani cisla 0
static bool new_line = false;   //Proměnná indikující nový řádek
static int buffer[BUFFER_SIZE]; //Pole pro uchovani velikosti odradkovani
static int act = 0;             //Ukazatel na vrchol pole buffer[]

//Inicializace scanneru
bool scanner_init(void) {
    line = 1;
    buffer[0] = 0;

    set_eof = false;
    new_line = true;

    if(tokenString(&val)) {
        return true;
    }
    return false;
}

//Uvolnění paměti scanneru
void scanner_free(void) {
    line = 0;
    free(val.string);
    set_eof = true;
}

//Kontrola zda načtené ID není klíčové slovo
token_typ check_kword(char *st) {
    assert(st);

    if(strcmp(st, "chr") == 0) {
        return token_chr;
    } else if(strcmp(st, "def") == 0) {
        return token_def;
    } else if(strcmp(st, "do") == 0) {
        return token_do;
    } else if(strcmp(st, "else") == 0) {
        return token_else;
    } else if(strcmp(st, "end") == 0) {
        return token_end;
    } else if(strcmp(st, "if") == 0) {
        return token_if;
    } else if(strcmp(st, "inputs") == 0) {
        return token_inputs;
    } else if(strcmp(st, "inputi") == 0) {
        return token_inputi;
    } else if(strcmp(st, "inputf") == 0) {
        return token_inputf;
    } else if(strcmp(st, "len") == 0) {
        return token_length;
    } else if(strcmp(st, "None") == 0) {
        return token_nil;
    } else if(strcmp(st, "ord") == 0) {
        return token_ord;
    } else if(strcmp(st, "print") == 0) {
        return token_print;
    } else if(strcmp(st, "substr") == 0) {
        return token_substr;
    } else if(strcmp(st, "then") == 0) {
        return token_then;
    } else if(strcmp(st, "while") == 0) {
        return token_while;
    } else if(strcmp(st, "pass") == 0) {
        return token_pass;
    } else if(strcmp(st, "return") == 0) {
        return token_return;
    } else {
        return token_nic;
    }
}

//Vytvoření nového tokenu a jeho inicializace
Token_t *create_new_token(void) {
    Token_t *token = (Token_t *)malloc(sizeof(Token_t));

    if(!token) {
        return NULL;
    }

    token->val.c = NULL;
    token->val.i = 0;
    token->val.d = 0.0;
    token->type = token_nic;
    token->line = 0;

    return token;
}

//Výčet možných stavů automatu scanneru
typedef enum {
    NIC,                    // 0
    TAB,                    // 1
    DED,                    // 2
    ID,                     // 3
    NUM,                    // 4
    DEC,                    // 5
    DEC_DOT,                // 6
    DEC_EXP,                // 7
    DEC_DOT_EXP,            // 8
    DEC_DOT_EXP_SIG,        // 9
    STRING,                 // 10
    STRING_ESC,             // 11
    STRING_ESC_1,           // 12
    STRING_ESC_2,           // 13
    STRING_DOC_START,       // 14
    STRING_DOC,             // 15
    STRING_DOC_ESC,         // 16
    STRING_DOC_ESC_1,       // 17
    STRING_DOC_ESC_2,       // 18
    STRING_DOC_END,         // 19
    COMMENT_LINE,           // 20
    EQUAL,                  // 21
    LESS,                   // 22
    GREAT,                  // 23
    NEG,                    // 24
} token_stav;

//Funkce, která načítá další vstup a zpracováváho
//Scanner
bool get_next_token(FILE *f, Token_t *token) {
    //Inicializace a kontrola zadaných hodnot
    assert(f != NULL);
    assert(token != NULL);

    token_stav stav = NIC;

    (val.string)[0] = '\0';
    val.length = 0;

    token->val.c = NULL;
    token->val.i = 0;
    token->val.d = 0.0;

    int stringEscVal = 0;
    int c = 0;

    //Pokud je konec souboru vrací se token konce souboru
    if(set_eof) {
        token->type = token_eof;
        return true;
    }

    //Dokud není konec souboru načte se další znak ze vstupu
    while(!set_eof) {
        c = getc(f);

        if(c == EOF) {
            c = '\n';

            if(stav == NIC) {
                set_eof = true;
            }
        }

        token->line = line;

        //Novy radek
        if(new_line) {
            stav = TAB;
            new_line = false;
        } 

        //Vynechávání mezer
        if(stav == NIC && isblank(c)) {
            continue;
        }

        //Samotný automat scanneru
        switch(stav) {
            //Načítání odsazení řádků
            case TAB:
                //Načtení počtu mezer
                if(isspace(c)) {
                    val_tab++;
                //Bílý znak, ale ne mezera
                } else if(isblank(c)) {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    fprintf(stderr, "LEX_ERR, Wrong tab format: 0x%02x at line: %d\n", c, line);
                    exit(1);
                //Načtení konce odsazení
                } else {
                    ungetc(c, f);
                    //Větší odsazení
                    if(buffer[act] < val_tab) {
                        act++;
                        if(buffer[act] = val_tab) {
                            exit(99);
                        }
                        token->type = token_indent;
                        stav = NIC;
                        return true;
                    //Menší odsazení
                    } else if(buffer[act] > val_tab) {
                        stav = DED;
                    //Stejné odsazení
                    } else {
                        stav = NIC;
                    }
                }
                break;
            
            //Menší odsazení
            case DED:
                //Výpis o kolik je odsazení menší
                if(buffer[act] > val_tab) {
                    token->type = token_dedent;
                    act--;
                    return true;
                //Nalezení odsazení
                } else if(buffer[act] == val_tab) {
                    stav = NIC;
                //Špatné odsazení
                } else if(buffer[act] < val_tab) {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong tab format at line: %d\n", line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Počáteční stav
            case NIC:
                if(c != '\n' && c != '/') {
                    empty_line = false;
                }

                if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = ID;  //nacteni znaku indikace ID
                } else if(isdigit(c)) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    if(c == '0') {  //Pokud byla zadaná číslice 0 musí za ní být . nebo konec číslice
                        set_nul = true;
                    }
                    stav = NUM; //nacteni číslice(1-9) indikace čísla
                } else if(c == '0') {   //nacteni číslice 0, jedná se o ERR
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong number format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                } else {
                    switch(c) {
                        case '=':
                            stav = EQUAL;   //nacteni rovnosti
                            break;
                        case '<':
                            stav = LESS;    //nacteni menší než
                            break;
                        case '>':
                            stav = GREAT;   //nacteni větší než
                            break;
                        case '#':
                            stav = COMMENT_LINE;    //nacteni začátku řádkového komentáře
                            break;
                        case '\'':
                            stav = STRING;  //nacteni začátku stringu
                            break;
                        case '"':
                            stav = STRING_DOC_START;
                            str_doc++;
                            break;
                        case '!':
                            stav = NEG; //nacteni negaco
                            break;
                        case '+':
                            token->type = token_plus;   //nacteni plus
                            return true;
                        case '-':
                            token->type = token_minus;  //nacteni minus
                            return true;
                        case '*':
                            token->type = token_mul;    //nacteni násobení
                            return true;
                        case '/':
                            token->type = token_div;    //nacteni dělení
                            return true;
                        case '(':
                            token->type = token_left_bracket;   //nacteni levé závorky
                            return true;
                        case ')':
                            token->type = token_right_bracket;  //nacteni pravé závorky
                            return true;
                        case ',':
                            token->type = token_comma;  //nacteni čárky
                            return true;
                        case '\n':  //nacteni konce řádku
                            line++;

                            new_line = true;

                            if(!empty_line) {
                                empty_line = true;
                                token->type = token_eol;
                                return true;
                            }
                            break;
                        case 13:    //nacteni
                            continue;

                        default:    //nacteni neznámého symbolu
                            fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                            printf("LEX_ERR, Unknown char: 0x%02x at line: %d\n", c, line);
                            return_eof_false(token);
                            exit(1);

                    }
                }
                break;

            //Řádkový komentář
            case COMMENT_LINE:
                if(c == '\n') {
                    stav = TAB;
                    ungetc(c, f);
                }
                break;

            //ID
            case ID:
                //Uložení znaku do proměnné st
                if(isalnum(c) || c == '_') {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                //Konec ID
                } else {
                    ungetc(c, f);

                    token_typ keyword;  //Pomocná proměnná
                    keyword = check_kword(val.string);  //Kontrola zda nejde o klíčové slovo
                    if(keyword != token_nic) {
                        token->type = keyword;
                        return true;
                    }
                    //Pokud nejde o klíčové slovo uloží se id do tokenu
                    token->type = token_id;
                    token->val.c = val.string;

                    //Uvedení proměnné st do původního stavu
                    if(!tokenString(&val)) {
                        val.string = token->val.c;
                        token->val.c = NULL;
                        return_eof_false(token);
                    }

                    return true;
                }
                break;

            //Číslo
            case NUM:
                //Načítání číslic
                if(isalnum(c) && set_nul) {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong number format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                } else if(isdigit(c)) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    set_nul = false;
                //Načtení desetinné tečky, jde o desetinné číslo
                } else if(c == '.') {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = DEC_DOT;
                    set_nul = false;
                //Načtení indikace exponentu, jde o číslo v exponenciálním tvaru
                } else if(c == 'e' || c == 'E') {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = DEC_EXP;
                    set_nul = false;
                //Uložení celého čísla
                } else {
                    ungetc(c, f);
                    int value = strtol(val.string, NULL, 10);
                    token->type = token_val_int;
                    token->val.i = value;
                    set_nul = false;
                    return true;
                }
                break;

            //Desetinné číslo
            case DEC_DOT:
                //Načtení první číslice desetinné části
                if(isdigit(c)) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = DEC;
                //Načtení chybného vstupu
                } else {
                    ungetc(c, f);
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong number format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Desetinná část čísla
            case DEC:
                //Načítání číslic desetinné části
                if(isdigit(c)) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                //Načtení indikace exponentu
                } else if(c == 'e' || c == 'E') {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = DEC_EXP;
                //Načtení konce desetinného čísla a jeho uložení
                } else {
                    ungetc(c, f);
                    float value = strtod(val.string, NULL);
                    token->type = token_val_float;
                    token->val.d = value;
                    return true;
                }
                break;

            //Znaménko exponentu čísla(zda se jedná o exponent se znaménkem nebo bez)
            case DEC_EXP:
                //Načtení první číslice exponenciálního čísla
                if(isdigit(c)) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = DEC_DOT_EXP;
                //Načtení znaku + nebo - exponentu
                } else if(c == '+' || c == '-') {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = DEC_DOT_EXP_SIG;
                //Načtení chyby vstupu
                } else {
                    ungetc(c, f);
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong number format 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Exponent desetinného čísla se znaménkem
            case DEC_DOT_EXP_SIG:
                if(isdigit(c)) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = DEC_DOT_EXP;
                } else {
                    ungetc(c, f);
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong number format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Exponent desetinného čísla
            case DEC_DOT_EXP:
                //Načítání číslic exponentu
                if(isdigit(c)) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                //Uložení desetinného čísla do tokenu
                } else {
                    ungetc(c, f);
                    float value = strtod(val.string, NULL);
                    token->type = token_val_float;
                    token->val.d = value;
                    return true;
                }
                break;

            //Řetězce
            case STRING:
                //Načtení zpětného lomítka
                if(c == '\\') {
                    stav = STRING_ESC;
                //Načtení tabulátoru a jeho uložení pomocí ASCII kódu
                } else if(c == '\t') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '9')) {
                        return_eof_false(token);
                    }
                //Načtení mezery a její uložení pomocí ASCII kódu
                } else if(c == ' ') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '3')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '2')) {
                        return_eof_false(token);
                    }
                //Načtení konce řetězce
                } else if(c == '\'') {
                    stav = NIC;
                    token->type = token_string;
                    //Uložení řetězce
                    token->val.c = val.string;

                    if(!tokenString(&val)) {
                        val.string = token->val.c;
                        token->val.c = NULL;
                        return_eof_false(token);
                    }
                    return true;
                //Přidání znaku řetězce do proměnné st
                } else if(c > 32) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                //Načtení chyby vstupu
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong number format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Řetězec se zpětným lomítkem
            case STRING_ESC:
                //Načtení indikace zadání znaku v 16 soustavě
                if(c == 'x') {
                    stringEscVal = 0;
                    stav = STRING_ESC_1;
                //Načtení tabulátoru a jeho uložení pomocí ASCII kódu
                } else if(c == 't') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '9')) {
                        return_eof_false(token);
                    }
                    stav = STRING;
                //Načtení zpětného lomítka a jeho uložení pomocí ASCII kódu
                } else if(c == '\\') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '9')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '2')) {
                        return_eof_false(token);
                    }
                    stav = STRING;
                //Načtení konce řádku a jeho uložení pomocí ASCII kódu
                } else if(c == 'n') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '1')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    stav = STRING;
                //Načtení uvozovek a jejich uložení
                } else if(c == '"') {
                    if(!tokenStringPridChar(&val, '"')) {
                        return_eof_false(token);
                    }
                    stav = STRING;
                //Načtení chyby vstupu
                } else {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = STRING;
                }
                break;

            //Hodnota znaku v 16 soustavě první číslice
            case STRING_ESC_1:
                if(isdigit(c) && c >= 'A' && c <= 'F') {
                    switch(c) {
                        case 'A':
                            stringEscVal = 10;
                            break;
                        case 'B':
                            stringEscVal = 11;
                            break;
                        case 'C':
                            stringEscVal = 12;
                            break;
                        case 'D':
                            stringEscVal = 13;
                            break;
                        case 'E':
                            stringEscVal = 14;
                            break;
                        case 'F':
                            stringEscVal = 15;
                            break;
                        default:
                            stringEscVal = c;
                            break;
                    }
                    stav = STRING_ESC_2;
                //Načtení chyby vstupu
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong string format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Hodnota znaku v 16 soustavě druhé číslice
            case STRING_ESC_2:
                if(isdigit(c) && c >= 'A' && c <= 'F') {
                    switch(c) {
                        case 'A':
                            stringEscVal += 16*10;
                            break;
                        case 'B':
                            stringEscVal += 16*11;
                            break;
                        case 'C':
                            stringEscVal += 16*12;
                            break;
                        case 'D':
                            stringEscVal += 16*13;
                            break;
                        case 'E':
                            stringEscVal += 16*14;
                            break;
                        case 'F':
                            stringEscVal += 16*15;
                            break;
                        default:
                            stringEscVal += 16*c;
                            break;
                    }
                    char a[3]; //Pomocná proměnná na rozdělení kódu znaku na tři části

                    //Uložení kódu znaku v ASCII
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0' + a[0])) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0' + a[1])) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0' + a[2])) {
                        return_eof_false(token);
                    }

                    stav = STRING;
                //Načtení chyby vstupu
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong string format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Dokumentacni retezec zacatek
            case STRING_DOC_START:
                if(c == '"') {
                    if(str_doc <= 3) {
                        str_doc++;
                    } else {
                        str_doc = 0;
                        stav = STRING_DOC;
                    }
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    fprintf(stderr, "LEX_ERR, Wrong doc. here string format: 0x%02x at line: %d\n", c, line);
                    exit(1);
                }
                break;

            //Dokumentacni retezec obsah
            case STRING_DOC:
                //Načtení zpětného lomítka
                if(c == '\\') {
                    stav = STRING_DOC_ESC;
                //Načtení tabulátoru a jeho uložení pomocí ASCII kódu
                } else if(c == '\t') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '9')) {
                        return_eof_false(token);
                    }
                //Načtení mezery a její uložení pomocí ASCII kódu
                } else if(c == ' ') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '3')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '2')) {
                        return_eof_false(token);
                    }
                //Načtení konce řádku a jeho uložení pomocí ASCII kódu
                } else if(c == '\n') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '1')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                //Načtení konce řetězce
                } else if(c == '"') {
                    stav = STRING_DOC_END;
                    str_doc++;
                //Přidání znaku řetězce do proměnné st
                } else if(c > 32) {
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                //Načtení chyby vstupu
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong number format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Řetězec se zpětným lomítkem
            case STRING_DOC_ESC:
                //Načtení indikace zadání znaku v 16 soustavě
                if(c == 'x') {
                    stringEscVal = 0;
                    stav = STRING_DOC_ESC_1;
                //Načtení tabulátoru a jeho uložení pomocí ASCII kódu
                } else if(c == 't') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '9')) {
                        return_eof_false(token);
                    }
                    stav = STRING_DOC;
                //Načtení zpětného lomítka a jeho uložení pomocí ASCII kódu
                } else if(c == '\\') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '9')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '2')) {
                        return_eof_false(token);
                    }
                    stav = STRING_DOC;
                //Načtení konce řádku a jeho uložení pomocí ASCII kódu
                } else if(c == 'n') {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '1')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0')) {
                        return_eof_false(token);
                    }
                    stav = STRING_DOC;
                //Načtení uvozovek a jejich uložení
                } else if(c == '"') {
                    if(!tokenStringPridChar(&val, '"')) {
                        return_eof_false(token);
                    }
                    stav = STRING_DOC;
                //Načtení chyby vstupu
                } else {
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, c)) {
                        return_eof_false(token);
                    }
                    stav = STRING_DOC;
                }
                break;

            //Hodnota znaku v 16 soustavě první číslice
            case STRING_DOC_ESC_1:
                if(isdigit(c) && c >= 'A' && c <= 'F') {
                    switch(c) {
                        case 'A':
                            stringEscVal = 10;
                            break;
                        case 'B':
                            stringEscVal = 11;
                            break;
                        case 'C':
                            stringEscVal = 12;
                            break;
                        case 'D':
                            stringEscVal = 13;
                            break;
                        case 'E':
                            stringEscVal = 14;
                            break;
                        case 'F':
                            stringEscVal = 15;
                            break;
                        default:
                            stringEscVal = c;
                            break;
                    }
                    stav = STRING_DOC_ESC_2;
                //Načtení chyby vstupu
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong string format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Hodnota znaku v 16 soustavě druhé číslice
            case STRING_DOC_ESC_2:
                if(isdigit(c) && c >= 'A' && c <= 'F') {
                    switch(c) {
                        case 'A':
                            stringEscVal += 16*10;
                            break;
                        case 'B':
                            stringEscVal += 16*11;
                            break;
                        case 'C':
                            stringEscVal += 16*12;
                            break;
                        case 'D':
                            stringEscVal += 16*13;
                            break;
                        case 'E':
                            stringEscVal += 16*14;
                            break;
                        case 'F':
                            stringEscVal += 16*15;
                            break;
                        default:
                            stringEscVal += 16*c;
                            break;
                    }
                    char a[3]; //Pomocná proměnná na rozdělení kódu znaku na tři části

                    //Uložení kódu znaku v ASCII
                    if(!tokenStringPridChar(&val, '\\')) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0' + a[0])) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0' + a[1])) {
                        return_eof_false(token);
                    }
                    if(!tokenStringPridChar(&val, '0' + a[2])) {
                        return_eof_false(token);
                    }

                    stav = STRING_DOC;
                //Načtení chyby vstupu
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong string format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Možný konec dokumentárního řetězce
            case STRING_DOC_END:
                //Načítání uvozovek
                if(c == '"' && str_doc < 3) {
                    str_doc++;
                //Načtní menšího počtu uvozovek než je na konec dokum. řetězce
                } else if(c != '"' && str_doc < 3) {
                    ungetc(c, f);

                    for(int i = 0; i < str_doc; i++) {
                        if(!tokenStringPridChar(&val, '"')) {
                            return_eof_false(token);
                        }
                    }

                    stav = STRING_DOC;
                //Načtení konce dokumentačního řetězce
                } else if(c != '"' && str_doc == 3) {
                    str_doc = 0;

                    ungetc(c, f);

                    stav = NIC;
                    token->type = token_string;
                    //Uložení řetězce
                    token->val.c = val.string;

                    if(!tokenString(&val)) {
                        val.string = token->val.c;
                        token->val.c = NULL;
                        return_eof_false(token);
                    }
                    return true;
                //Načtení chyby vstupu
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong string format: 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Menší
            case LESS:
                //Menší nebo rovno
                if(c == '=') {
                    token->type = token_less_equal;
                    return true;
                //Menší než
                } else {
                    ungetc(c, f);
                    token->type = token_less;
                }
                return true;
                break;

            //Větší
            case GREAT:
                //Větší nebo rovno
                if(c == '='){
                    token->type = token_great_equal;
                    return true;
                //Větší než
                } else {
                    ungetc(c, f);
                    token->type = token_great;
                }
                return true;
                break;

            //Není rovno
            case NEG:
                if(c == '=') {
                    token->type = token_neg_equal;
                    return true;
                //Načtení chyby vstupu
                } else {
                    fprintf(stderr, "%s:%d %s:%d\n", "ERROR", LEX_ERR, "at line", line);
                    printf("LEX_ERR, Wrong number format 0x%02x at line: %d\n", c, line);
                    return_eof_false(token);
                    exit(1);
                }
                break;

            //Rovno
            case EQUAL:
                if(c == '=') {
                    token->type = token_equal_equal;
                //Rovno
                } else {
                    ungetc(c, f);
                    token->type = token_equal;
                }
                return true;
                break;
        }
    }

    //Chyba vstupu
    if(stav != NIC) {
        fprintf(stderr, "%s:%d %s: %d\n", "ERROR", LEX_ERR, "at line", line);
        printf("LEX_ERR, Unexpectid end of file\n");
        exit(1);
    }

    return true;
}
