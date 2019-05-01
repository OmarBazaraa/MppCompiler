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
        if (!(rhs->analyze(context) & lhs->analyze(context))) {
            // Note that I used a bitwise AND to execute both lhs and rhs expressions
            return false;
        }

        type = lhs->type;
        reference = lhs->reference;
        isConst = lhs->isConst;

        if (type == DTYPE_FUNC_PTR) {
            context->printError("assignment of function '" + reference->header() + "'", lhs->loc);
            return false;
        }
        if (reference == NULL) {
            context->printError("lvalue required as left operand of assignment", lhs->loc);
            return false;
        }
        if (reference && isConst) {
            context->printError("assignment of read-only variable '" + reference->header() + "'", lhs->loc);
            return false;
        }
        if (rhs->type == DTYPE_VOID || rhs->type == DTYPE_FUNC_PTR) {
            context->printError("invalid conversion from '" + rhs->getType() + "' to '" + lhs->getType() + "'", rhs->loc);
            return false;
        }

        return true;
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + "(" + lhs->toString() + " = " + rhs->toString() + ")";
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
        if (!(lhs->analyze(context) & rhs->analyze(context))) {
            // Note that I used a bitwise AND to execute both lhs and rhs expressions
            return false;
        }

        type = max(lhs->type, rhs->type);
        isConst = (lhs->isConst && rhs->isConst);

        if (lhs->type == DTYPE_VOID || lhs->type == DTYPE_FUNC_PTR ||
            rhs->type == DTYPE_VOID || rhs->type == DTYPE_FUNC_PTR ||
            Utils::isBitwiseOpr(opr) && (lhs->type == DTYPE_FLOAT || rhs->type == DTYPE_FLOAT)) {
            context->printError("invalid operands of types '" + lhs->getType() + "' and '" + rhs->getType() + "' to " + getOpr(), loc);
            return false;
        }

        return true;
    }

    virtual string getOpr() {
        return "binary operator '" + Utils::oprToStr(opr) + "'";
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + "(" + lhs->toString() + " " + Utils::oprToStr(opr) + " " + rhs->toString() + ")";
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
        if (!expr->analyze(context)) {
            return false;
        }

        type = expr->type;
        reference = expr->reference;
        isConst = expr->isConst;

        if (expr->type == DTYPE_VOID || expr ->type == DTYPE_FUNC_PTR ||
            expr->type == DTYPE_FLOAT && Utils::isBitwiseOpr(opr)) {
            context->printError("invalid operand of type '" + expr->getType() + "' to " + getOpr(), loc);
            return false;
        }

        if (Utils::isLvalueOpr(opr)) {
            if (reference == NULL) {
                context->printError("lvalue required as an operand of increment/decrement operator", expr->loc);
                return false;
            }
            if (reference && isConst) {
                context->printError("increment/decrement of read-only variable '" + reference->header() + "'", expr->loc);
                return false;
            }
        }

        return true;
    }

    virtual string getOpr() {
        return "unary operator '" + Utils::oprToStr(opr) + "'";
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "(";

        if (opr == OPR_SUF_INC || opr == OPR_SUF_DEC) {
            ret += expr->toString() + Utils::oprToStr(opr);
        } else {
            ret += Utils::oprToStr(opr) + expr->toString();
        }

        return ret += ")";
    }
};

#endif