%{
#include <vector>
#include <string>

#include "../parse_tree/parse_tree.h"
#include "parser.hpp"

using namespace std;

#define ADVANCE_CURSOR      (curCursorPos += (curTokenLen = yyleng))

//
// Functions prototypes
//
void saveLocation();
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
<INITIAL>"int"                      saveLocation(); return TYPE_INT;
<INITIAL>"float"                    saveLocation(); return TYPE_FLOAT;
<INITIAL>"char"                     saveLocation(); return TYPE_CHAR;
<INITIAL>"bool"                     saveLocation(); return TYPE_BOOL;
<INITIAL>"void"                     saveLocation(); return TYPE_VOID;

%{
// Branch tokens
%}
<INITIAL>"const"                    saveLocation(); return CONST;
<INITIAL>"if"                       saveLocation(); return IF;
<INITIAL>"else"                     saveLocation(); return ELSE;
<INITIAL>"switch"                   saveLocation(); return SWITCH;
<INITIAL>"case"                     saveLocation(); return CASE;
<INITIAL>"default"                  saveLocation(); return DEFAULT;
<INITIAL>"for"                      saveLocation(); return FOR;
<INITIAL>"do"                       saveLocation(); return DO;
<INITIAL>"while"                    saveLocation(); return WHILE;
<INITIAL>"break"                    saveLocation(); return BREAK;
<INITIAL>"continue"                 saveLocation(); return CONTINUE;
<INITIAL>"return"                   saveLocation(); return RETURN;

%{
// Operators
%}
<INITIAL>"++"                       saveLocation(); return INC;
<INITIAL>"--"                       saveLocation(); return DEC;
<INITIAL>"=="                       saveLocation(); return EQUAL;
<INITIAL>"!="                       saveLocation(); return NOT_EQUAL;
<INITIAL>">="                       saveLocation(); return GREATER_EQUAL;
<INITIAL>"<="                       saveLocation(); return LESS_EQUAL;
<INITIAL>"<<"                       saveLocation(); return SHL;
<INITIAL>">>"                       saveLocation(); return SHR;
<INITIAL>"&&"                       saveLocation(); return LOGICAL_AND;
<INITIAL>"||"                       saveLocation(); return LOGICAL_OR;
<INITIAL>[-+*/%&|^~!<>=(){},:;]     saveLocation(); return yytext[0];

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
<INITIAL>{WHITESPACE}               ADVANCE_CURSOR; // Ignore whitespace
<INITIAL>{LINE_COMMENT}             ADVANCE_CURSOR; // Ignore comments
<INITIAL>"/*"                       ADVANCE_CURSOR; BEGIN BLOCK_COMMENT;
<BLOCK_COMMENT>"*/"                 ADVANCE_CURSOR; BEGIN INITIAL;
<BLOCK_COMMENT>.                    ADVANCE_CURSOR; // Ignore block comments

%%

// =====================================================================================================
// User Subroutines Section
// ========================

void saveLocation() {
    yylval.location.lineNum = curLineNum;
    yylval.location.pos = curCursorPos;
    yylval.location.len = yyleng;

    ADVANCE_CURSOR;
}

void saveToken() {
    yylval.token.value = strdup(yytext);
    yylval.token.loc.lineNum = curLineNum;
    yylval.token.loc.pos = curCursorPos;
    yylval.token.loc.len = yyleng;

    ADVANCE_CURSOR;
}

int yywrap() {
    return 1;
}