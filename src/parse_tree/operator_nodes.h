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

    AssignOprNode(const Location& loc, IdentifierNode* name, ExpressionNode* value) : ExpressionNode(loc) {
        this->name = name;
        this->value = value;
    }

    virtual ~AssignOprNode() {
        if (name) delete name;
        if (value) delete value;
    }

    virtual bool analyze(ScopeContext* context) {
        bool ret = name->analyze(context) && value->analyze(context);

        type = name->type;

        Symbol* ptr = context->getSymbol(name->name);

        if (ptr != NULL) {
            if (dynamic_cast<Var*>(ptr) == NULL) {
                context->printError("assignment of function '" + ptr->header() + "'", value->loc);
                ret = false;
            }
            else if (((Var*) ptr)->isConst) {
                context->printError("assignment of read-only variable '" + ptr->header() + "'", value->loc);
                ret = false;
            }
        }

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

    BinaryOprNode(const Location& loc, Operator opr, ExpressionNode* lhs, ExpressionNode* rhs) : ExpressionNode(loc) {
        this->opr = opr;
        this->lhs = lhs;
        this->rhs = rhs;
    }

    virtual ~BinaryOprNode() {
        if (lhs) delete lhs;
        if (rhs) delete rhs;
    }

    virtual bool analyze(ScopeContext* context) {
        bool ret = lhs->analyze(context) && rhs->analyze(context);

        if (lhs->type == DTYPE_VOID || rhs->type == DTYPE_VOID) {
            context->printError("invalid operands of types '" + 
                Utils::dtypeToStr(lhs->type) + "' and '" + Utils::dtypeToStr(rhs->type) +
                "' to binary operator '" + Utils::oprToStr(opr) + "'", loc);
            ret = false;
        }

        type = max(lhs->type, rhs->type); // Note that lhs & rhs types are computed after calling analyze function

        return ret;
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
        bool ret = expr->analyze(context);

        if (expr->type == DTYPE_VOID) {
            context->printError("invalid operand of type '" + 
                Utils::dtypeToStr(expr->type) + "' to unary 'operator" + Utils::oprToStr(opr) + "'", loc);
            ret = false;
        }

        type = expr->type;

        return ret;
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