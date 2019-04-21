%{
// ==================
// Definition Section
// ==================

/**
 * This is the Definition section of the Parser Specification file.
 *
 * Yacc copies the material between "%{" and "%}"
 * directly to the generated C file,
 * so you may write any valid C code here. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

// Function declarations
extern int yylex();
void yyerror(char* s);

// =====================================================================================================
%}

// Define the types of the symbols
%union {
    double val;
    struct symbol_table* sym_ptr;
}

// Token Definitions
// Yacc defines the token names in the parser as C preprocessor names in "y.tab.h"
%token <sym_ptr>    NAME
%token <val>        NUMBER

// Assign types for non-terminal symbols
%type <val>         expression

// Assign precendence & associativity
// Note that order matters here
%left '-' '+'
%left '*' '/'
%nonassoc UMINUM

// Start Symbol
// By default, its the first LHS symbol in the rules section
%start statement_list

%%

// =============
// Rules Section
// =============

// the default action that Yacc performs after every reduction,
// before running any explicit action code,
// assigns the value $1 to $$. 

statement_list:     statement '\n'
        |           statement_list statement '\n'
        ;

statement:          NAME '=' expression             { $1->value = $3; }
        |           expression                      { printf("= %lf\n", $1); }
        ;

expression:         expression '+' expression       { $$ = $1 + $3; }
        |           expression '-' expression       { $$ = $1 - $3; }
        |           expression '*' expression       { $$ = $1 * $3; }
        |           expression '/' expression       {
                                                        if ($3 != 0)
                                                            $$ = $1 / $3;
                                                        else
                                                            yyerror("division by zero!");
                                                    }
        |           '-' expression %prec UMINUM     { $$ = -$2; }
        |           '(' expression ')'              { $$ = $2; }
        |           NUMBER                          { $$ = $1; }
        |           NAME                            { $$ = $1->value; }
        ;

%%

// ========================
// User Subroutines Section
// ========================

extern int running;

int main () {
    yyparse();
    return 0;
}

void yyerror(char* s) {
    fprintf(stderr, "%s\n", s); 
}

struct symbol_table* symbol_look(char* s) {
    for (int i = 0; i < SYM_MAX; ++i) {
        struct symbol_table* p = &symTable[i];

        if (p->name != NULL && strcmp(s, p->name) == 0) {
            return p;
        }

        if (p->name == NULL) {
            p->name = strdup(s);
            return p;
        }
    }

    yyerror("Too many symbols");
    exit(0);
}