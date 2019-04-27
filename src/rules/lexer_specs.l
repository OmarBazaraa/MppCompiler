%{
#include "../parse_tree/parse_tree.h"

#include "parser.hpp"

#define SYNC_CURSOR     (curCursorPos += (curTokenLen = yyleng))

char* curLine = NULL;
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
\n                                  {
                                        curLineNum++;
                                        curCursorPos = 0;
                                    }

^.*\n                               {
                                        if (curLine) delete curLine;
                                        curLine = strdup(yytext);
                                        REJECT;
                                    }

%{
// Data types
%}
<INITIAL>"int"                      SYNC_CURSOR; return TYPE_INT;
<INITIAL>"float"                    SYNC_CURSOR; return TYPE_FLOAT;
<INITIAL>"char"                     SYNC_CURSOR; return TYPE_CHAR;
<INITIAL>"bool"                     SYNC_CURSOR; return TYPE_BOOL;
<INITIAL>"void"                     SYNC_CURSOR; return TYPE_VOID;

%{
// Branch tokens
%}
<INITIAL>"const"                    SYNC_CURSOR; return CONST;
<INITIAL>"if"                       SYNC_CURSOR; return IF;
<INITIAL>"else"                     SYNC_CURSOR; return ELSE;
<INITIAL>"switch"                   SYNC_CURSOR; return SWITCH;
<INITIAL>"case"                     SYNC_CURSOR; return CASE;
<INITIAL>"default"                  SYNC_CURSOR; return DEFAULT;
<INITIAL>"for"                      SYNC_CURSOR; return FOR;
<INITIAL>"do"                       SYNC_CURSOR; return DO;
<INITIAL>"while"                    SYNC_CURSOR; return WHILE;
<INITIAL>"break"                    SYNC_CURSOR; return BREAK;
<INITIAL>"continue"                 SYNC_CURSOR; return CONTINUE;
<INITIAL>"return"                   SYNC_CURSOR; return RETURN;

%{
// Operators
%}
<INITIAL>"++"                       SYNC_CURSOR; return INC;
<INITIAL>"--"                       SYNC_CURSOR; return DEC;
<INITIAL>"=="                       SYNC_CURSOR; return EQUAL;
<INITIAL>"!="                       SYNC_CURSOR; return NOT_EQUAL;
<INITIAL>">="                       SYNC_CURSOR; return GREATER_EQUAL;
<INITIAL>"<="                       SYNC_CURSOR; return LESS_EQUAL;
<INITIAL>"<<"                       SYNC_CURSOR; return SHL;
<INITIAL>">>"                       SYNC_CURSOR; return SHR;
<INITIAL>"&&"                       SYNC_CURSOR; return LOGICAL_AND;
<INITIAL>"||"                       SYNC_CURSOR; return LOGICAL_OR;
<INITIAL>[-+*/%&|^~!<>=(){},:;]     SYNC_CURSOR; return yytext[0];

%{
// Values
%}
<INITIAL>{INTEGER}                  SYNC_CURSOR; yylval.valueInt = atoi(yytext);     return INTEGER;
<INITIAL>{REAL}                     SYNC_CURSOR; yylval.valueFloat = atof(yytext);   return FLOAT;
<INITIAL>(\'.\')                    SYNC_CURSOR; yylval.valueChar = yytext[1];       return CHAR;
<INITIAL>"true"                     SYNC_CURSOR; yylval.valueBool = true;            return BOOL;
<INITIAL>"false"                    SYNC_CURSOR; yylval.valueBool = false;           return BOOL;
<INITIAL>{IDENTIFIER}               SYNC_CURSOR; yylval.valueIden = strdup(yytext);  return IDENTIFIER;

%{
// Others
%}
<INITIAL>{WHITESPACE}               SYNC_CURSOR; // Ignore whitespace
<INITIAL>{LINE_COMMENT}             SYNC_CURSOR; // Ignore comments
<INITIAL>"/*"                       SYNC_CURSOR; BEGIN BLOCK_COMMENT;
<BLOCK_COMMENT>"*/"                 SYNC_CURSOR; BEGIN INITIAL;
<BLOCK_COMMENT>.                    SYNC_CURSOR; // Ignore block comments

%%

// =====================================================================================================
// User Subroutines Section
// ========================

int yywrap() {
    return 1;
}