#ifndef __STATEMENT_NODES_H_
#define __STATEMENT_NODES_H_

#include "../basic_nodes.h"
#include "../expressions/expression_nodes.h"


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

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "{\n";
        for (int i = 0; i < statements.size(); ++i) {
            ret += statements[i]->toString(ind + 4) + "\n";
        }
        return ret += string(ind, ' ') + "}";
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* generationContext);
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

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + var.header();
        if (value) {
            ret += " = " + value->toString();
        }
        return ret;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* generationContext);
};

#endif