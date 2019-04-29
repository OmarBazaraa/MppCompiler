#ifndef __OPERATOR_NODES_H_
#define __OPERATOR_NODES_H_

#include "basic_nodes.h"
#include "value_nodes.h"


/**
 * The node class holding an assignment operator in the parse tree.
 */
struct AssignOprNode : public ExpressionNode {
    ExpressionNode* lhs;
    ExpressionNode* rhs;

    AssignOprNode(const Location& loc, ExpressionNode* lhs, ExpressionNode* rhs) : ExpressionNode(loc) {
        this->lhs = lhs;
        this->rhs = rhs;
    }

    virtual ~AssignOprNode() {
        if (lhs) delete lhs;
        if (rhs) delete rhs;
    }

    virtual bool analyze(ScopeContext* context) {
        bool ret = rhs->analyze(context) && lhs->analyze(context);

        type = lhs->type;
        reference = lhs->reference;

        if (reference) {
            if (dynamic_cast<Var*>(reference) == NULL) {
                context->printError("assignment of function '" + reference->header() + "'", rhs->loc);
                ret = false;
            }
            else if (((Var*) reference)->isConst) {
                context->printError("assignment of read-only variable '" + reference->header() + "'", rhs->loc);
                ret = false;
            }
        }
        else {
            context->printError("lvalue required as left operand of assignment", lhs->loc);
            ret = false;
        }

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "(";
        lhs->print(0);
        cout << " = ";
        rhs->print(0);
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

        type = max(lhs->type, rhs->type); // Note that lhs & rhs types are computed after calling analyze function

        if (lhs->type == DTYPE_VOID || rhs->type == DTYPE_VOID) {
            context->printError("invalid operands of types '" + 
                Utils::dtypeToStr(lhs->type) + "' and '" + Utils::dtypeToStr(rhs->type) +
                "' to binary operator '" + Utils::oprToStr(opr) + "'", loc);
            ret = false;
        }

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

        type = expr->type;
        reference = expr->reference;

        if (expr->type == DTYPE_VOID) {
            context->printError("invalid operand of type '" + 
                Utils::dtypeToStr(expr->type) + "' to unary 'operator" + Utils::oprToStr(opr) + "'", loc);
            ret = false;
        }

        if (opr == OPR_SUF_INC || opr == OPR_PRE_INC || opr == OPR_SUF_DEC || opr == OPR_PRE_DEC) {
            if (reference) {
                if (dynamic_cast<Var*>(reference) == NULL) {
                    context->printError("increment/decrement of function '" + reference->header() + "'", expr->loc);
                    ret = false;
                }
                else if (((Var*) reference)->isConst) {
                    context->printError("increment/decrement of read-only variable '" + reference->header() + "'", expr->loc);
                    ret = false;
                }
            }
            else {
                context->printError("lvalue required as an operand of increment/decrement", expr->loc);
                ret = false;
            }
        }

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