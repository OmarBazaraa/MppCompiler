#ifndef __FUNCTION_NODES_H_
#define __FUNCTION_NODES_H_

#include "basic_nodes.h"
#include "statement_nodes.h"


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

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + type->toString() + " " + func.identifier + "(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + paramList[i]->toString();
        }
        ret += ")\n";
        ret += body->toString(ind);
        return ret;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* generationContext);
};

/**
 * The node class holding a function call expression in the parse tree.
 */
struct FunctionCallNode : public ExpressionNode {
    IdentifierNode* name;
    ExprList argList;
    Func* func;

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

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + func->identifier + "(";
        for (int i = 0; i < argList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + argList[i]->toString();
        }
        return ret += ")";
    }

    virtual bool analyze(ScopeContext* context, bool valueUsed);

    virtual string generateQuad(GenerationContext* generationContext);
};

/**
 * The node class holding a return statement in the parse tree.
 */
struct ReturnStmtNode : public StatementNode {
    ExpressionNode* value;
    Func* func;

    ReturnStmtNode(const Location& loc, ExpressionNode* value) : StatementNode(loc) {
        this->value = value;
    }

    virtual ~ReturnStmtNode() {
        if (value) delete value;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "return";
        if (value) {
            ret += " " + value->toString();
        }
        return ret;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* generationContext);
};

#endif