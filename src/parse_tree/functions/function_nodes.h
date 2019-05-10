#ifndef __FUNCTION_NODES_H_
#define __FUNCTION_NODES_H_

#include "../basic_nodes.h"
#include "../statements/statement_nodes.h"


/**
 * The node class holding a function in the parse tree.
 */
struct FunctionNode : public DeclarationNode {
    VarList paramList;
    BlockNode* body;

    FunctionNode(TypeNode* type, IdentifierNode* ident, const VarList& paramList, BlockNode* body)
            : DeclarationNode(type->loc) {
        this->type = type;
        this->ident = ident;
        this->paramList = paramList;
        this->body = body;
        this->initialized = true;
    }

    virtual ~FunctionNode() {
        if (type) delete type;
        if (ident) delete ident;
        if (body) delete body;

        for (int i = 0; i < paramList.size(); ++i) {
            delete paramList[i];
        }
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + type->toString() + " " + ident->toString() + "(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + paramList[i]->toString();
        }
        ret += ")\n";
        ret += body->toString(ind);
        return ret;
    }

    virtual string declaredHeader() {
        string ret = type->toString() + " " + ident->name + "(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + paramList[i]->type->toString();
        }
        ret += ")";
        return ret;
    }

    virtual string declaredType() {
        string ret = type->toString() + "(*)(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + paramList[i]->type->toString();
        }
        ret += ")";
        return ret;
    }
};

/**
 * The node class holding a function call expression in the parse tree.
 */
struct FunctionCallNode : public ExpressionNode {
    IdentifierNode* ident;
    ExprList argList;
    FunctionNode* func;

    FunctionCallNode(IdentifierNode* ident, const ExprList& argList) : ExpressionNode(ident->loc) {
        this->ident = ident;
        this->argList = argList;
    }

    virtual ~FunctionCallNode() {
        if (ident) delete ident;

        for (int i = 0; i < argList.size(); ++i) {
            delete argList[i];
        }
    }

    virtual bool analyze(ScopeContext* context, bool valueUsed);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + ident->name + "(";
        for (int i = 0; i < argList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + argList[i]->toString();
        }
        ret += ")";
        return ret;
    }
};

/**
 * The node class holding a return statement in the parse tree.
 */
struct ReturnStmtNode : public StatementNode {
    ExpressionNode* value;
    FunctionNode* func;

    ReturnStmtNode(const Location& loc, ExpressionNode* value) : StatementNode(loc) {
        this->value = value;
    }

    virtual ~ReturnStmtNode() {
        if (value) delete value;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "return";
        if (value) {
            ret += " " + value->toString();
        }
        return ret;
    }
};

#endif