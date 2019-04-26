%{
#include <iostream>
#include <string>

#include "../parse_tree/parse_tree.h"

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
    IfNode*             ifStmt;
    SwitchNode*         switchStmt;
    CaseStmtNode*       caseStmt;
    WhileNode*          whileStmt;
    DoWhileNode*        doWhileStmt;
    ForNode*            forStmt;
    FunctionNode*       function;
    FunctionCallNode*   functionCall;
    ReturnStmtNode*     returnStmt;
    VarList*            paramList;
    ExprList*           argList;
    CaseList*           caseList;
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
%token <token> ELSE
%token <token> SWITCH
%token <token> CASE
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
%type <statement>       stmt branch_body case_body for_init_stmt
%type <varDecl>         var_decl var_decl_uninit var_decl_init
%type <ifStmt>          if_stmt unmatched_if_stmt matched_if_stmt
%type <switchStmt>      switch_stmt
%type <caseStmt>        case_stmt
%type <caseList>        switch_body case_stmt_block case_stmt_list
%type <whileStmt>       while_stmt
%type <doWhileStmt>     do_while_stmt
%type <forStmt>         for_stmt for_header
%type <function>        function function_header
%type <functionCall>    function_call
%type <returnStmt>      return_stmt
%type <paramList>       param_list param_list_ext
%type <argList>         arg_list arg_list_ext
%type <expression>      expression for_expr
%type <dtype>           type

// =====================================================================================================
// Precendence & Associativity
// ===========================

// Note that order matters here
%right      '='
%left       LOGICAL_OR
%left       LOGICAL_AND
%left       '|'
%left       '^'
%left       '&'
%left       EQUAL NOT_EQUAL
%left       LESS_EQUAL GREATER_EQUAL '<' '>'
%left       SHIFT_RIGHT SHIFT_LEFT
%left       '-' '+'
%left       '*' '/' '%'
%right      '!' '~'
%right      U_PLUS U_MINUM
%right      PRE_INC PRE_DEC
%left       SUF_INC SUF_DEC

%nonassoc   IF_UNMAT
%nonassoc   ELSE

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
    |               BREAK ';'               { $$ = new BreakStmtNode(); }
    |               CONTINUE ';'            { $$ = new ContinueStmtNode(); }
    |               var_decl ';'            { $$ = $1; }
    |               expression ';'          { $$ = $1; }
    |               if_stmt                 { $$ = $1; }
    |               switch_stmt             { $$ = $1; }
    |               while_stmt              { $$ = $1; }
    |               do_while_stmt ';'       { $$ = $1; }
    |               for_stmt                { $$ = $1; }
    |               function                { $$ = $1; }
    |               return_stmt ';'         { $$ = $1; }
    ;

branch_body:        stmt                    { $$ = $1; }
    |               stmt_block              { $$ = $1; }
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
    
//  TODO - fix postfix inc/dec
//  |               expression INC %prec SUF_INC            { $$ = new UnaryOprNode(OPR_SUF_INC, $2); }
//  |               expression DEC %prec SUF_DEC            { $$ = new UnaryOprNode(OPR_SUF_DEC, $2); }

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
// If Rules
//

if_stmt:            unmatched_if_stmt
    |               matched_if_stmt
    ;

unmatched_if_stmt:  IF '(' expression ')' branch_body %prec IF_UNMAT    { $$ = new IfNode($3, $5); }
    ;

matched_if_stmt:    IF '(' expression ')' branch_body ELSE branch_body  { $$ = new IfNode($3, $5, $7); }
    ;

// ------------------------------------------------------------
//
// Switch Rules
//

switch_stmt:        SWITCH '(' expression ')' switch_body   { $$ = new SwitchNode($3, *$5); delete $5; }
    ;

switch_body:        case_stmt_block                         { $$ = $1; }
    ;

case_stmt_block:    '{' '}'                                 { $$ = new CaseList(); }
    |               '{' case_stmt_list '}'                  { $$ = $2; }
    ;

case_stmt_list:     case_stmt                               { $$ = new CaseList(); $$->push_back($1); }
    |               case_stmt_list case_stmt                { $$ = $1; $$->push_back($2); }
    ;

case_stmt:          CASE expression ':' case_body           { $$ = new CaseStmtNode($2, $4); }
    |               DEFAULT ':' case_body                   { $$ = new CaseStmtNode(NULL, $3, true); }
    ;

case_body:          /* epsilon */                           { $$ = NULL; }
    |               stmt_list                               { $$ = $1; }
    ;

// ------------------------------------------------------------
//
// While Rules
//

while_stmt:         WHILE '(' expression ')' branch_body                { $$ = new WhileNode($3, $5); }
    ;

do_while_stmt:      DO branch_body WHILE '(' expression ')'             { $$ = new DoWhileNode($5, $2); }
    ;

// ------------------------------------------------------------
//
// For Rules
//

for_stmt:           for_header branch_body                              { $$ = $1; $$->body = $2; }
    ;

for_header:         FOR '(' for_init_stmt ';' for_expr ';' for_expr ')' { $$ = new ForNode($3, $5, $7, NULL); }
    ;

for_init_stmt:      /* epsilon */                                       { $$ = NULL; }
    |               var_decl                                            { $$ = $1; }
    |               expression                                          { $$ = $1; }
    ;

for_expr:           /* epsilon */                                       { $$ = NULL; }
    |               expression                                          { $$ = $1; }
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

return_stmt:        RETURN expression                   { $$ = new ReturnStmtNode($2); }

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

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s); 
}