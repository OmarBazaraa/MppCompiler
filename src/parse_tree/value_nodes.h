#ifndef __VALUE_NODES_H_
#define __VALUE_NODES_H_

#include "../context/generation_context.h"
#include "basic_nodes.h"


/**
 * The node class holding a data type in the parse tree.
 */
struct TypeNode : public StatementNode {
    DataType type;

    TypeNode(const Location& loc, DataType type) : StatementNode(loc) {
        this->type = type;
    }

    virtual bool analyze(ScopeContext* context) {
        return true;
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + Utils::dtypeToStr(type);
    }

    virtual string generateQuad(GenerationContext* generationContext) {
        return "";
    }
};

/**
 * The node class holding a value in the parse tree.
 */
struct ValueNode : public ExpressionNode {
    string value;

    ValueNode(const Location& loc, DataType type, const char* value) : ExpressionNode(loc) {
        this->type = type;
        this->value = value;
        this->isConst = true;
    }

    virtual bool analyze(ScopeContext* context, bool valueUsed) {
        used = valueUsed;
        return true;
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + value;
    }

    virtual string generateQuad(GenerationContext* generationContext) {
        string ret;
        if (used) {
            ret += Utils::oprToQuad(OPR_PUSH, type) + " " + value + "\n";
        }
        return ret;
    }
};

/**
 * The node class holding an identifier in the parse tree.
 */
struct IdentifierNode : public ExpressionNode {
    string name;

    IdentifierNode(const Location& loc, const char* name) : ExpressionNode(loc) {
        this->name = name;
    }

    virtual bool analyze(ScopeContext* context, bool valueUsed) {
        Symbol* ptr = context->getSymbol(name);

        if (ptr == NULL) {
            context->printError("'" + name + "' was not declared in this scope", loc);
            return false;
        }

        reference = ptr;

        if (dynamic_cast<Func*>(ptr)) {
            type = DTYPE_FUNC_PTR;
        } else {
            type = ptr->type;
            isConst = ((Var*) ptr)->isConst;
        }

        used = valueUsed;

        if (used) {
            ptr->used = true;
        }

        return true;
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + name;
    }

    virtual string generateQuad(GenerationContext* generationContext) {
        string ret;
        if (used) {
            ret += Utils::oprToQuad(OPR_PUSH, type) + " " + reference->alias + "\n";;
        }
        return ret;
    }
};

#endif