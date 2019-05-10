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

    virtual string generateQuad(GenerationContext* context);

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
    bool constant;

    VarDeclarationNode(TypeNode* type, IdentifierNode* ident, ExpressionNode* value = NULL, bool constant = false)
            : DeclarationNode(type->loc) {
        this->type = type;
        this->ident = ident;
        this->value = value;
        this->constant = constant;
        this->initialized = (value != NULL);
    }

    virtual ~VarDeclarationNode() {
        if (type) delete type;
        if (ident) delete ident;
        if (value) delete value;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + declaredHeader();
        if (value) {
            ret += " = " + value->toString();
        }
        return ret;
    }

    virtual string declaredHeader() {
        return (constant ? "const " : "") + type->toString() + " " + ident->name;
    }

    virtual string declaredType() {
        return (constant ? "const " : "") + type->toString();
    }
};

/**
 * The node class holding multiple variables or constants declaration statement in the parse tree.
 */
struct MultiVarDeclarationNode : public StatementNode {
    TypeNode* type;
    VarList vars;
    bool constant;

    MultiVarDeclarationNode(VarDeclarationNode* var) {
        this->type = new TypeNode(*(var->type));
        this->constant = var->constant;
        this->vars.push_back(var);
    }

    virtual ~MultiVarDeclarationNode() {
        delete type;
        for (int i = 0; i < vars.size(); ++i) {
            delete vars[i];
        }
    }

    virtual void addVar(IdentifierNode* ident, ExpressionNode* value = NULL) {
        vars.push_back(new VarDeclarationNode(new TypeNode(*type), ident, value, constant));
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + type->toString();
        for (int i = 0; i < vars.size(); ++i) {
            ret += (i > 0 ? ", " : "") + vars[i]->toString();
        }
        return ret;
    }
};

#endif