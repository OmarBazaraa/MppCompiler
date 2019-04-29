#ifndef __OPERATOR_NODES_H_
#define __OPERATOR_NODES_H_

#include "basic_nodes.h"
#include "value_nodes.h"


/**
 * The node class holding an assignment operator in the parse tree.
 */
struct AssignOprNode : public ExpressionNode {
    IdentifierNode* name;
    ExpressionNode* value;

    AssignOprNode(IdentifierNode* name, ExpressionNode* value) : ExpressionNode(name->loc) {
        this->name = name;
        this->value = value;
    }

    virtual ~AssignOprNode() {
        if (name) delete name;
        if (value) delete value;
    }

    virtual bool analyze(ScopeContext* context) {
        bool ret = true;

        Symbol* ptr = context->getSymbol(name->name);

        if (ptr == NULL) {
            context->printError("'" + name->name + "' was not declared in this scope", loc);
            ret = false;
        }
        else if (dynamic_cast<Var*>(ptr) == NULL) {
            context->printError("assignment of function '" + ptr->header() + "'", value->loc);
            ret = false;
        }

        ret &= value->analyze(context);

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "(";
        name->print(0);
        cout << " = ";
        value->print(0);
        cout << ")";
    }
};

/**
 * The node class holding a binary operator in the parse tree.
 */
struct BinaryOprNode : public ExpressionNode {
    Operator opr;
    ExpressionNode* lhs;
    ExpressionNode* rhs;

    BinaryOprNode(Operator opr, ExpressionNode* lhs, ExpressionNode* rhs) : ExpressionNode(lhs->loc) {
        this->opr = opr;
        this->lhs = lhs;
        this->rhs = rhs;
    }

    virtual ~BinaryOprNode() {
        if (lhs) delete lhs;
        if (rhs) delete rhs;
    }

    virtual bool analyze(ScopeContext* context) {
        return lhs->analyze(context) && rhs->analyze(context);
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "(";
        lhs->print(0);
        cout << ' ' << Utils::oprToStr(opr) << ' ';
        rhs->print(0);
        cout << ")";
    }
};

/**
 * The node class holding a unary operator in the parse tree.
 */
struct UnaryOprNode : public ExpressionNode {
    Operator opr;
    ExpressionNode* expr;

    UnaryOprNode(const Location& loc, Operator opr, ExpressionNode* expr) : ExpressionNode(loc) {
        this->opr = opr;
        this->expr = expr;
    }

    virtual ~UnaryOprNode() {
        if (expr) delete expr;
    }

    virtual bool analyze(ScopeContext* context) {
        return expr->analyze(context);
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "(";

        if (opr == OPR_SUF_INC || opr == OPR_SUF_DEC) {
            expr->print(0);
            cout << Utils::oprToStr(opr);
        } else {
            cout << Utils::oprToStr(opr);
            expr->print(0);
        }

        cout << ")";
    }
};

#endif