%{
#include <vector>
#include <string>

#include "../parse_tree/parse_tree.h"
#include "parser.hpp"

using namespace std;

//
// Functions prototypes
//
void saveToken();

//
// Global variables
//
int curLineNum = 1;
int curCursorPos = 0;
int curTokenLen = 0;
%}

%{
// =====================================================================================================
// Start States
// ============
%}

%s BLOCK_COMMENT

%{
// =====================================================================================================
// Pattern Definitions (Substitutions)
// ===================================

// TODO: add (-|+)? to numberic values
%}

DIGIT               [0-9]
INTEGER             [0-9]+
FLOAT               (([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))
EXP                 [eE][-+]?{INTEGER}
REAL                ({INTEGER}{EXP}|{FLOAT}({EXP})?)
LETTER              [a-zA-Z_]
IDENTIFIER          {LETTER}({LETTER}|{DIGIT})*
LINE_COMMENT        "//"(.)*
WHITESPACE          [ \t]+

%%

%{
// =====================================================================================================
// Rules Section
// =============
%}

%{
// Token localization
%}
\n                                  curLineNum++; curCursorPos = 0;

%{
// Data types
%}
<INITIAL>"int"                      saveToken(); return TYPE_INT;
<INITIAL>"float"                    saveToken(); return TYPE_FLOAT;
<INITIAL>"char"                     saveToken(); return TYPE_CHAR;
<INITIAL>"bool"                     saveToken(); return TYPE_BOOL;
<INITIAL>"void"                     saveToken(); return TYPE_VOID;

%{
// Branch tokens
%}
<INITIAL>"const"                    saveToken(); return CONST;
<INITIAL>"if"                       saveToken(); return IF;
<INITIAL>"else"                     saveToken(); return ELSE;
<INITIAL>"switch"                   saveToken(); return SWITCH;
<INITIAL>"case"                     saveToken(); return CASE;
<INITIAL>"default"                  saveToken(); return DEFAULT;
<INITIAL>"for"                      saveToken(); return FOR;
<INITIAL>"do"                       saveToken(); return DO;
<INITIAL>"while"                    saveToken(); return WHILE;
<INITIAL>"break"                    saveToken(); return BREAK;
<INITIAL>"continue"                 saveToken(); return CONTINUE;
<INITIAL>"return"                   saveToken(); return RETURN;

%{
// Operators
%}
<INITIAL>"++"                       saveToken(); return INC;
<INITIAL>"--"                       saveToken(); return DEC;
<INITIAL>"=="                       saveToken(); return EQUAL;
<INITIAL>"!="                       saveToken(); return NOT_EQUAL;
<INITIAL>">="                       saveToken(); return GREATER_EQUAL;
<INITIAL>"<="                       saveToken(); return LESS_EQUAL;
<INITIAL>"<<"                       saveToken(); return SHL;
<INITIAL>">>"                       saveToken(); return SHR;
<INITIAL>"&&"                       saveToken(); return LOGICAL_AND;
<INITIAL>"||"                       saveToken(); return LOGICAL_OR;
<INITIAL>[-+*/%&|^~!<>=(){},:;]     saveToken(); return yytext[0];

%{
// Values
%}
<INITIAL>{INTEGER}                  saveToken(); return INTEGER;
<INITIAL>{REAL}                     saveToken(); return FLOAT;
<INITIAL>(\'.\')                    saveToken(); return CHAR;
<INITIAL>"true"                     saveToken(); return BOOL;
<INITIAL>"false"                    saveToken(); return BOOL;
<INITIAL>{IDENTIFIER}               saveToken(); return IDENTIFIER;

%{
// Others
%}
<INITIAL>{WHITESPACE}               saveToken(); // Ignore whitespace
<INITIAL>{LINE_COMMENT}             saveToken(); // Ignore comments
<INITIAL>"/*"                       saveToken(); BEGIN BLOCK_COMMENT;
<BLOCK_COMMENT>"*/"                 saveToken(); BEGIN INITIAL;
<BLOCK_COMMENT>.                    saveToken(); // Ignore block comments

%%

// =====================================================================================================
// User Subroutines Section
// ========================

void saveToken() {
    yylval.token.value = strdup(yytext);
    yylval.token.lineNum = curLineNum;
    yylval.token.pos = curCursorPos;
    yylval.token.len = curTokenLen = yyleng;

    curCursorPos += yyleng;
}

int yywrap() {
    return 1;
}