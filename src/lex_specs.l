%{
// ==================
// Definition Section
// ==================

/**
 * This is the Definition section of Lex Specification file.
 *
 * Lex copies the material between "%{" and "%}"
 * directly to the generated C file,
 * so you may write any valid C code here. 
 */

// =====================================================================================================
%}

%{
// ============
// Start States
// ============

/**
 * You enter a new state with a BEGIN statement.
 * For instance, to change to the FNAME state we used the statement "BEGIN FNAME;".
 * To change back to the default state, we use "BEGIN 0".
 * (The default, state zero, is also known as INITIAL.)
 */
%}

%s COMMENT_BEGIN

%{
// =====================================================================================================
%}

%{
// ===================================
// Pattern Definitions (Substitutions)
// ===================================
%}

WHITESPACE          [ \t\n]+

%{
// =====================================================================================================
%}

%%

%{
// =============
// Rules section
// =============

// NOTE: comments in rules section must begin with whitespace,
// unless it is in a literal block like this one.

/**
 * Each rule is made up of two parts: a pattern and an action, separated by whitespace.
 * The lexer that lex generates will execute the action when it recognizes the pattern.
 */
%}

{WHITESPACE}                        ;
-?(([0-9]+)|([0-9]*\.[0-9]+))       printf("Number of %d chars, lineno: %d\n", yyleng, yylineno);

%{
// =====================================================================================================
%}

%%

// ========================
// User Subroutines Section
// ========================

void main() {
    yylex();
}

int yywrap() {
    return 1;
}