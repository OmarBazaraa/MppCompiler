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

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* generationContext);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "{\n";
        for (int i = 0; i < statements.size(); ++i) {
            ret += statements[i]->toString(ind + 4) + "\n";
        }
        ret += string(ind, ' ') + "}";
        return ret;
    }
};

/**
 * The node class holding a variable or constant declaration statement in the parse tree.
 */
struct VarDeclarationNode : public DeclarationNode {
    ExpressionNode* value;
    bool isConst;

    VarDeclarationNode(TypeNode* type, IdentifierNode* ident, ExpressionNode* value = NULL, bool isConst = false)
            : DeclarationNode(type->loc) {
        this->type = type;
        this->ident = ident;
        this->value = value;
        this->isConst = isConst;
    }

    virtual ~VarDeclarationNode() {
        if (type) delete type;
        if (ident) delete ident;
        if (value) delete value;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* generationContext);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + declaredHeader();
        if (value) {
            ret += " = " + value->toString();
        }
        return ret;
    }

    virtual string declaredHeader() {
        return (isConst ? "const " : "") + type->toString() + " " + ident->name;
    }

    virtual string declaredType() {
        return type->toString();
    }
};

#endif