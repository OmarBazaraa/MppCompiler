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

// Function declarations
extern int yylex();
void yyerror(char* s);

// Global variables
double varTable[26];

// =====================================================================================================
%}

// Define the types of the symbols
%union {
    double val;
    int idx;
}

// Token Definitions
// Yacc defines the token names in the parser as C preprocessor names in "y.tab.h"
%token <idx> NAME
%token <val> NUMBER

// Assign types for non-terminal symbols
%type <val> expression

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

statement:          NAME '=' expression             { varTable[$1] = $3; }
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
        |           NAME                            { $$ = varTable[$1]; }
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