#ifndef __STATEMENT_NODES_H_
#define __STATEMENT_NODES_H_

#include "basic_nodes.h"
#include "value_nodes.h"


/**
 * The node class holding a block of code in the parse tree.
 */
struct BlockNode : public StatementNode {
    StmtList statements;

    BlockNode() {

    }

    BlockNode(const Location& loc) : StatementNode(loc) {

    }

    BlockNode(const Location& loc, const StmtList& statements) : StatementNode(loc) {
        this->statements = statements;
    }

    virtual ~BlockNode() {
        for (int i = 0; i < statements.size(); ++i) {
            delete statements[i];
        }
    }

    virtual bool analyze(Context* context) {
        bool ret = true;

        context->addScope(SCOPE_BLOCK);

        for (int i = 0; i < statements.size(); ++i) {
            ret &= statements[i]->analyze(context);
        }

        context->popScope();

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "{" << endl;
        for (int i = 0; i < statements.size(); ++i) {
            statements[i]->print(ind + 4);
            cout << endl;
        }
        cout << string(ind, ' ') << "}";
    }
};

/**
 * The node class holding a variable or constant declaration statement in the parse tree.
 */
struct VarDeclarationNode : public StatementNode {
    TypeNode* type;
    IdentifierNode* name;
    ExpressionNode* value;
    Var var;

    VarDeclarationNode(TypeNode* type, IdentifierNode* name, ExpressionNode* value = NULL, bool isConst = false) : StatementNode(type->loc) {
        this->type = type;
        this->name = name;
        this->value = value;

        this->var.type = type->type;
        this->var.identifier = name->name;
        this->var.isConst = isConst;
        this->var.isInitialized = (value != NULL);
    }

    virtual ~VarDeclarationNode() {
        if (type) delete type;
        if (name) delete name;
        if (value) delete value;
    }

    virtual bool analyze(Context* context) {
        bool ret = true;

        if (type->type == DTYPE_VOID) {
            context->printError("variable or field '" + name->name + "' declared void", name->loc);
            ret = false;
        }
        else if (!context->declareSymbol(&var)) {
            context->printError("'" + var.header() + "' redeclared", name->loc);
            ret = false;
        }
        else if (var.isConst && value == NULL) {
            context->printError("uninitialized const '" + name->name + "'", name->loc);
            ret = false;
        }

        // TODO: add flag to know if the variable is a function parameter or nor
        // TODO: check value expression        

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << var.header();

        if (value) {
            cout << " = ";
            value->print(0);
        }
    }
};

/**
 * The node class holding a break statement in the parse tree.
 */
struct BreakStmtNode : public StatementNode {

    BreakStmtNode(const Location& loc) : StatementNode(loc) {

    }

    virtual ~BreakStmtNode() {

    }

    virtual bool analyze(Context* context) {
        if (!context->hasBreakScope()) {
            context->printError("break statement not within loop or switch", loc);
            return false;
        }

        return true;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "break;";
    }
};

/**
 * The node class holding a continue statement in the parse tree.
 */
struct ContinueStmtNode : public StatementNode {

    ContinueStmtNode(const Location& loc) : StatementNode(loc) {

    }

    virtual ~ContinueStmtNode() {

    }

    virtual bool analyze(Context* context) {
        if (!context->hasLoopScope()) {
            context->printError("continue statement not within loop", loc);
            return false;
        }

        return true;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "continue;";
    }
};

/**
 * The node class holding a return statement in the parse tree.
 */
struct ReturnStmtNode : public StatementNode {
    ExpressionNode* value;

    ReturnStmtNode(const Location& loc, ExpressionNode* value) : StatementNode(loc) {
        this->value = value;
    }

    virtual ~ReturnStmtNode() {
        if (value) delete value;
    }

    virtual bool analyze(Context* context) {
        if (!context->hasFunctionScope()) {
            context->printError("return statement not within function", loc);
            return false;
        }

        // TODO: check return type
        
        return true;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "return";
        if (value) {
            cout << " ";
            value->print(0);
        }
        cout << ";";
    }
};

/**
 * The node class holding a case statement in the parse tree.
 * 
 * TODO: redefine switch case grammar.
 */
struct CaseStmtNode : public StatementNode {
    ExpressionNode* expr;
    StmtList body;
    bool isDefault;

    CaseStmtNode(const Location& loc, ExpressionNode* expr, const StmtList& body, bool isDefault = false) : StatementNode(loc) {
        this->expr = expr;
        this->body = body;
        this->isDefault = isDefault;
    }

    virtual ~CaseStmtNode() {
        if (expr) {
            delete expr;
        }
        for (int i = 0; i < body.size(); ++i) {
            delete body[i];
        }
    }

    virtual bool analyze(Context* context) {
        return true;
    }

    virtual void print(int ind = 0) {
        if (isDefault) {
            cout << string(ind, ' ') << "default:" << endl;
        } else {
            cout << string(ind, ' ') << "case ";
            expr->print(0);
            cout << ":" << endl;
        }
        for (int i = 0; i < body.size(); ++i) {
            body[i]->print(ind + 4);
            cout << endl;
        }
    }
};

#endif