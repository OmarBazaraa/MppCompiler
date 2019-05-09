%{
#include <vector>
#include <string>

#include "../parse_tree/parse_tree.h"
#include "parser.hpp"

using namespace std;

#define ADVANCE_CURSOR      (curLoc.pos += yyleng)

//
// Functions prototypes
//
void saveLocation();
void saveToken();

//
// Global variables
//
Location curLoc = {1,0,0};
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
%}

DIGIT               [0-9]
INTEGER             [0-9]+
FLOAT               (([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))
EXP                 ([eE][-+]?{INTEGER})
REAL                ({INTEGER}{EXP}|{FLOAT}{EXP}?)
LETTER              [a-zA-Z_]
IDENTIFIER          {LETTER}({LETTER}|{DIGIT})*
LINE_COMMENT        "//"(.)*

%%

%{
// =====================================================================================================
// Rules Section
// =============
%}

%{
// Token localization
%}
\n                                  curLoc.lineNum++; curLoc.pos = 0;
\r                                  ;
\t                                  curLoc.pos += 4;
" "                                 curLoc.pos++;

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
<INITIAL>[-+*/%&|^~!<>=(){}[\],:;]  saveLocation(); return yytext[0];


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
<INITIAL>{LINE_COMMENT}             ADVANCE_CURSOR; // Ignore comments
<INITIAL>"/*"                       ADVANCE_CURSOR; BEGIN BLOCK_COMMENT;
<BLOCK_COMMENT>"*/"                 ADVANCE_CURSOR; BEGIN INITIAL;
<BLOCK_COMMENT>.                    ADVANCE_CURSOR; // Ignore block comments

<INITIAL>.                          saveLocation(); return yytext[0];

%%

// =====================================================================================================
// User Subroutines Section
// ========================

void saveLocation() {
    curLoc.len = yyleng;

    yylval.location = curLoc;
    yylval.location.pos++;

    ADVANCE_CURSOR;
}

void saveToken() {
    curLoc.len = yyleng;

    yylval.token.value = strdup(yytext);
    yylval.token.loc = curLoc;
    yylval.token.loc.pos++;

    ADVANCE_CURSOR;
}

int yywrap() {
    return 1;
}