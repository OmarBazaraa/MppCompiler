%{
#include <iostream>
#include <string>

#include "parse_tree.h"

using namespace std;

//
// Functions prototypes
//
extern int yylex();
void yyerror(const char* s);

//
// Global variables
//
Node* programRoot = NULL;
%}

// =====================================================================================================
// Symbol Types
// ============

%union {
    Node*               node;
    BlockNode*          block;
    StatementNode*      statement;
    VarDeclarationNode* varDecl;
    FunctionNode*       function;
    FunctionCallNode*   functionCall;
    VarList*            paramList;
    ExprList*           argList;
    ExpressionNode*     expression;
    DataType            dtype;
    int                 token;
    int                 valueInt;
    float               valueFloat;
    char                valueChar;
    bool                valueBool;
    char*               valueIden;
}

// =====================================================================================================
// Tokens Definition
// =================

// Data types
%token <dtype> TYPE_INT
%token <dtype> TYPE_FLOAT
%token <dtype> TYPE_CHAR
%token <dtype> TYPE_BOOL
%token <dtype> TYPE_VOID

// Keywords
%token <token> CONST
%token <token> IF
%token <token> SWITCH
%token <token> DEFAULT
%token <token> FOR
%token <token> DO
%token <token> WHILE
%token <token> BREAK
%token <token> CONTINUE
%token <token> RETURN

// Operators
%token <token> INC
%token <token> DEC
%token <token> EQUAL
%token <token> NOT_EQUAL
%token <token> GREATER_EQUAL
%token <token> LESS_EQUAL
%token <token> SHIFT_LEFT
%token <token> SHIFT_RIGHT
%token <token> LOGICAL_AND
%token <token> LOGICAL_OR

// Values
%token <valueIden>  IDENTIFIER
%token <valueInt>   INTEGER
%token <valueFloat> FLOAT
%token <valueChar>  CHAR
%token <valueBool>  BOOL

// =====================================================================================================
// Non-terminal Symbols Types
// ==========================

%type <block>           program stmt_list stmt_block
%type <statement>       stmt
%type <varDecl>         var_decl var_decl_uninit var_decl_init
%type <function>        function function_header
%type <functionCall>    function_call
%type <paramList>       param_list param_list_ext
%type <argList>         arg_list arg_list_ext
%type <expression>      expression
%type <dtype>           type

// =====================================================================================================
// Precendence & Associativity
// ===========================

// Note that order matters here
%right  '='
%left   LOGICAL_OR
%left   LOGICAL_AND
%left   '|'
%left   '^'
%left   '&'
%left   EQUAL NOT_EQUAL
%left   LESS_EQUAL GREATER_EQUAL '<' '>'
%left   SHIFT_RIGHT SHIFT_LEFT
%left   '-' '+'
%left   '*' '/' '%'
%right  '!' '~'
%right  U_PLUS U_MINUM
%right  PRE_INC PRE_DEC
%left   SUF_INC SUF_DEC

%%

// =====================================================================================================
// Rules Section
// =============

program:            /* epsilon */           { programRoot = new BlockNode(); }
    |               stmt_list               { programRoot = $1; }
    ;

stmt_list:          stmt                    { $$ = new BlockNode(); $$->statements.push_back($1); }
    |               stmt_block              { $$ = new BlockNode(); $$->statements.push_back($1); }
    |               stmt_list stmt          { $1->statements.push_back($2); }
    |               stmt_list stmt_block    { $1->statements.push_back($2); }
    ;

stmt_block:         '{' '}'                 { $$ = new BlockNode(); }
    |               '{' stmt_list '}'       { $$ = $2; }
    ;

stmt:               ';'                     { $$ = new StatementNode(); }
    |               var_decl ';'            { $$ = $1; }
    |               expression ';'          { $$ = $1; }
    |               function                { $$ = $1; }
    ;

// ------------------------------------------------------------
//
// Declaration Rules
//

var_decl:           var_decl_uninit
    |               var_decl_init
    ;

var_decl_uninit:    type IDENTIFIER                         { $$ = new VarDeclarationNode($1, $2); }
    |               CONST type IDENTIFIER                   { $$ = new VarDeclarationNode($2, $3, NULL, true); }
    ;

var_decl_init:      type IDENTIFIER '=' expression          { $$ = new VarDeclarationNode($1, $2, $4); }
    |               CONST type IDENTIFIER '=' expression    { $$ = new VarDeclarationNode($2, $3, $5, true); }
    ;

// ------------------------------------------------------------
//
// Expression Rules
//

expression:         IDENTIFIER '=' expression               { $$ = new AssignOprNode($1, $3); }

    |               expression '+' expression               { $$ = new BinaryOprNode(OPR_ADD, $1, $3); }
    |               expression '-' expression               { $$ = new BinaryOprNode(OPR_SUB, $1, $3); }
    |               expression '*' expression               { $$ = new BinaryOprNode(OPR_MUL, $1, $3); }
    |               expression '/' expression               { $$ = new BinaryOprNode(OPR_DIV, $1, $3); }
    |               expression '%' expression               { $$ = new BinaryOprNode(OPR_MOD, $1, $3); }
    |               expression '&' expression               { $$ = new BinaryOprNode(OPR_AND, $1, $3); }
    |               expression '|' expression               { $$ = new BinaryOprNode(OPR_OR, $1, $3); }
    |               expression '^' expression               { $$ = new BinaryOprNode(OPR_XOR, $1, $3); }
    |               expression SHIFT_LEFT expression        { $$ = new BinaryOprNode(OPR_SHL, $1, $3); }
    |               expression SHIFT_RIGHT expression       { $$ = new BinaryOprNode(OPR_SHR, $1, $3); }
    |               expression LOGICAL_AND expression       { $$ = new BinaryOprNode(OPR_LOGICAL_AND, $1, $3); }
    |               expression LOGICAL_OR expression        { $$ = new BinaryOprNode(OPR_LOGICAL_OR, $1, $3); }
    |               expression '>' expression               { $$ = new BinaryOprNode(OPR_GREATER, $1, $3); }
    |               expression GREATER_EQUAL expression     { $$ = new BinaryOprNode(OPR_GREATER_EQUAL, $1, $3); }
    |               expression '<' expression               { $$ = new BinaryOprNode(OPR_LESS, $1, $3); }
    |               expression LESS_EQUAL expression        { $$ = new BinaryOprNode(OPR_LESS_EQUAL, $1, $3); }
    |               expression EQUAL expression             { $$ = new BinaryOprNode(OPR_EQUAL, $1, $3); }
    |               expression NOT_EQUAL expression         { $$ = new BinaryOprNode(OPR_NOT_EQUAL, $1, $3); }

    |               INC expression %prec PRE_INC            { $$ = new UnaryOprNode(OPR_PRE_INC, $2); }
    |               DEC expression %prec PRE_DEC            { $$ = new UnaryOprNode(OPR_PRE_DEC, $2); }
    
    // TODO: fix postfix inc/dec
    // |               expression INC %prec SUF_INC            { $$ = new UnaryOprNode(OPR_SUF_INC, $2); }
    // |               expression DEC %prec SUF_DEC            { $$ = new UnaryOprNode(OPR_SUF_DEC, $2); }

    |               '+' expression %prec U_PLUS             { $$ = new UnaryOprNode(OPR_U_PLUS, $2); }
    |               '-' expression %prec U_MINUM            { $$ = new UnaryOprNode(OPR_U_MINUS, $2); }
    |               '~' expression                          { $$ = new UnaryOprNode(OPR_NOT, $2); }
    |               '!' expression                          { $$ = new UnaryOprNode(OPR_LOGICAL_NOT, $2); }

    |               '(' expression ')'                      { $$ = $2; }

    |               INTEGER                                 { $$ = new IntNode($1); }
    |               FLOAT                                   { $$ = new FloatNode($1); }
    |               CHAR                                    { $$ = new CharNode($1); }
    |               BOOL                                    { $$ = new BoolNode($1); }
    |               IDENTIFIER                              { $$ = new VarNode($1); }
    |               function_call                           { $$ = $1; }
    ;

// ------------------------------------------------------------
//
// Function Rules
//

function:           function_header stmt_block          { $$ = $1; $$->body = $2; }
    ;

function_header:    type IDENTIFIER '(' param_list ')'  { $$ = new FunctionNode($1, $2, *$4, NULL); delete $4; }
    ;

param_list:         /* epsilon */                       { $$ = new VarList(); }
    |               var_decl                            { $$ = new VarList(); $$->push_back($1); }
    |               param_list_ext ',' var_decl         { $$ = $1; $$->push_back($3); }
    ;

param_list_ext:     var_decl                            { $$ = new VarList(); $$->push_back($1); }
    |               param_list_ext ',' var_decl         { $$ = $1; $$->push_back($3); }
    ;

function_call:      IDENTIFIER '(' arg_list ')'         { $$ = new FunctionCallNode($1, *$3); delete $3; }
    ;

arg_list:           /* epsilon */                       { $$ = new ExprList(); }
    |               expression                          { $$ = new ExprList(); $$->push_back($1); }
    |               arg_list_ext ',' expression         { $$ = $1; $$->push_back($3); }
    ;

arg_list_ext:       expression                          { $$ = new ExprList(); $$->push_back($1); }
    |               arg_list_ext ',' expression         { $$ = $1; $$->push_back($3); }
    ;

// ------------------------------------------------------------
//
// Other Rules
//

type:               TYPE_INT        { $$ = DTYPE_INT; }
    |               TYPE_FLOAT      { $$ = DTYPE_FLOAT; }
    |               TYPE_CHAR       { $$ = DTYPE_CHAR; }
    |               TYPE_BOOL       { $$ = DTYPE_BOOL; }
    |               TYPE_VOID       { $$ = DTYPE_VOID; }
    ;

%%

// =====================================================================================================
// User Subroutines Section
// ========================

int main () {
    yyparse();

    programRoot->print();
    delete programRoot;

    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s); 
}