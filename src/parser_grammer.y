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

// =====================================================================================================
%}

// Token Definitions
// Yacc defines the token names in the parser as C preprocessor names in "y.tab.h"
%token NAME NUMBER

// Start Symbol
// By default, its the first LHS symbol in the rules section
%start statement

%%

// =============
// Rules Section
// =============

statement:      NAME '=' expression
        |       expression              { printf("= %d\n", $1); }
        ;

// the default action that Yacc performs after every reduction,
// before running any explicit action code,
// assigns the value $1 to $$. 

expression:     expression '+' NUMBER   { $$ = $1 + $3; }
        |       expression '-' NUMBER   { $$ = $1 - $3; }
        |       NUMBER                  { $$ = $1; }
        ;

%%

// ========================
// User Subroutines Section
// ========================

extern FILE* yyin;
extern int running;

int main () {
    while (running) {
        yyparse();
    }

    return 0;
}

void yyerror(char* s) {
    fprintf(stderr, "%s\n", s); 
}