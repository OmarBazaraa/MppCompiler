#ifndef __FUNCTION_NODES_H_
#define __FUNCTION_NODES_H_

#include "basic_nodes.h"
#include "statement_nodes.h"
#include "value_nodes.h"


/**
 * The node class holding a function in the parse tree.
 */
struct FunctionNode : public StatementNode {
    TypeNode* type;
    IdentifierNode* name;
    VarList paramList;
    BlockNode* body;
    Func func;

    FunctionNode(TypeNode* type, IdentifierNode* name, const VarList& paramList, BlockNode* body) : StatementNode(type->loc) {
        this->type = type;
        this->name = name;
        this->paramList = paramList;
        this->body = body;

        this->func.type = type->type;
        this->func.identifier = name->name;

        for (int i = 0; i < paramList.size(); ++i) {
            this->func.paramList.push_back(paramList[i]->var);
        }
    }

    virtual ~FunctionNode() {
        if (type) delete type;
        if (name) delete name;
        if (body) delete body;

        for (int i = 0; i < paramList.size(); ++i) {
            delete paramList[i];
        }
    }

    virtual bool analyze(ScopeContext* context) {
        bool ret = true;

        if (!context->declareSymbol(&func)) {
            context->printError("'" + func.header() + "' redeclared", name->loc);
            ret = false;
        }

        context->addScope(SCOPE_FUNCTION);

        for (int i = 0; i < paramList.size(); ++i) {
            ret &= paramList[i]->analyze(context);
        }
        
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << func.header();
        body->print(ind);
    }
};

/**
 * The node class holding a function call expression in the parse tree.
 */
struct FunctionCallNode : public ExpressionNode {
    IdentifierNode* name;
    ExprList argList;

    FunctionCallNode(IdentifierNode* name, const ExprList& argList) : ExpressionNode(name->loc) {
        this->name = name;
        this->argList = argList;
    }

    virtual ~FunctionCallNode() {
        if (name) delete name;

        for (int i = 0; i < argList.size(); ++i) {
            delete argList[i];
        }
    }

    virtual bool analyze(ScopeContext* context) {
        bool ret = true;

        Symbol* ptr = context->getSymbol(name->name);

        if (ptr == NULL) {
            context->printError("'" + name->name + "' was not declared in this scope", loc);
            ret = false;
        }

        if (dynamic_cast<Func*>(ptr) == NULL) {
            context->printError("'" + name->name + "' cannot be used as a function", loc);
            ret = false;
        }

        for (int i = 0; i < argList.size(); ++i) {
            ret &= argList[i]->analyze(context);
        }

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ');
        name->print(0);
        cout << "(";

        if (argList.size()) {
            argList[0]->print(0);
        }

        for (int i = 1; i < argList.size(); ++i) {
            cout << ", ";
            argList[i]->print(0);
        }

        cout << ")";
    }
};

#endif