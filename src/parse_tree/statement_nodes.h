#ifndef __STATEMENT_NODES_H_
#define __STATEMENT_NODES_H_

#include "basic_nodes.h"
#include "value_nodes.h"


/**
 * The node class holding a block of code in the parse tree.
 */
struct BlockNode : public StatementNode {
    StmtList statements;

    BlockNode() {}

    BlockNode(const Location& loc) : StatementNode(loc) {}

    BlockNode(const Location& loc, const StmtList& statements) : StatementNode(loc) {
        this->statements = statements;
    }

    virtual ~BlockNode() {
        for (int i = 0; i < statements.size(); ++i) {
            delete statements[i];
        }
    }

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("block is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_BLOCK);

        for (int i = 0; i < statements.size(); ++i) {
            ret &= statements[i]->analyze(context);
        }

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "{\n";
        for (int i = 0; i < statements.size(); ++i) {
            ret += statements[i]->toString(ind + 4) + "\n";
        }
        return ret += string(ind, ' ') + "}";
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

    virtual bool analyze(ScopeContext* context) {
        bool ret = true;

        if (type->type == DTYPE_VOID) {
            context->printError("variable or field '" + name->name + "' declared void", name->loc);
            ret = false;
        }
        else if (!context->declareSymbol(&var)) {
            context->printError("'" + var.header() + "' redeclared", name->loc);
            ret = false;
        }

        if (value) {
            context->initializeVar = true;
            ret &= value->analyze(context);
            context->initializeVar = false;
        }

        if (context->declareFuncParams && var.isInitialized) {
            context->printError("default function parameters are not allowed", value->loc);
            ret = false;
        }
        else if (var.isConst && !var.isInitialized && !context->declareFuncParams) {
            context->printError("uninitialized const '" + name->name + "'", name->loc);
            ret = false;
        }

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + var.header();
        if (value) {
            ret += " = " + value->toString(0);
        }
        return ret;
    }
};

/**
 * The node class holding a break statement in the parse tree.
 */
struct BreakStmtNode : public StatementNode {

    BreakStmtNode(const Location& loc) : StatementNode(loc) {}

    virtual bool analyze(ScopeContext* context) {
        if (!context->hasBreakScope()) {
            context->printError("break-statement not within loop or switch", loc);
            return false;
        }

        return true;
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + "break";
    }
};

/**
 * The node class holding a continue statement in the parse tree.
 */
struct ContinueStmtNode : public StatementNode {

    ContinueStmtNode(const Location& loc) : StatementNode(loc) {}

    virtual bool analyze(ScopeContext* context) {
        if (!context->hasLoopScope()) {
            context->printError("continue-statement not within loop", loc);
            return false;
        }

        return true;
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + "continue";
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

    virtual bool analyze(ScopeContext* context) {
        Func* func = (Func*) context->getFunctionSymbol();

        if (func == NULL) {
            context->printError("return-statement not within function", loc);
            return false;
        }

        bool ret = true;
        
        if (value) {
            ret = value->analyze(context);
        }

        if (value == NULL && func->type != DTYPE_VOID) {
            context->printError("return-statement with no value, in function returning '" + Utils::dtypeToStr(func->type) + "'", loc);
            ret = false;
        }
        else if (value != NULL && func->type == DTYPE_VOID) {
            context->printError("return-statement with a value, in function returning 'void'", value->loc);
            ret = false;
        }
        
        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "return";
        if (value) {
            ret += " " + value->toString(0);
        }
        return ret;
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

    virtual bool analyze(ScopeContext* context) {
        return true;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + (isDefault ? "default:\n" : "case " + expr->toString() + ":\n");

        for (int i = 0; i < body.size(); ++i) {
            ret += body[i]->toString(ind + 4) + "\n";
        }

        return ret;
    }
};

#endif