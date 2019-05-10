#ifndef __OPERATOR_NODES_H_
#define __OPERATOR_NODES_H_

#include "../basic_nodes.h"


/**
 * An expression container class.
 */
struct ExprContainerNode : public ExpressionNode {
    ExpressionNode* expr;

    ExprContainerNode(const Location& loc, ExpressionNode* expr) : ExpressionNode(loc) {
        this->expr = expr;
    }

    virtual ~ExprContainerNode() {
        if (expr) delete expr;
    }

    virtual int getConstIntValue() {
        return expr->getConstIntValue();
    }

    virtual bool analyze(ScopeContext* context, bool valueUsed);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        return expr->toString(ind);
    }
};

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

    virtual bool analyze(ScopeContext* context, bool valueUsed);

    virtual string generateQuad(GenerationContext* context);

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

    virtual int getConstIntValue();

    virtual bool analyze(ScopeContext* context, bool valueUsed);

    virtual string generateQuad(GenerationContext* context);

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

    virtual int getConstIntValue();

    virtual bool analyze(ScopeContext* context, bool valueUsed);

    virtual string generateQuad(GenerationContext* context);

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

        ret += ")";
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

    virtual int getConstIntValue();

    virtual bool analyze(ScopeContext* context, bool valueUsed);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + name;
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
        this->constant = true;
    }

    virtual int getConstIntValue();

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + value;
    }
};

#endif