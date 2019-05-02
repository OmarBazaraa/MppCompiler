#ifndef __OPERATOR_NODES_H_
#define __OPERATOR_NODES_H_

#include "../context/generation_context.h"
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

        if (lhs->type == DTYPE_FUNC_PTR) {
            context->printError("assignment of function '" + lhs->reference->header() + "'", lhs->loc);
            return false;
        }
        if (lhs->reference == NULL) {
            context->printError("lvalue required as left operand of assignment", lhs->loc);
            return false;
        }
        if (lhs->reference && lhs->isConst) {
            context->printError("assignment of read-only variable '" + lhs->reference->header() + "'", lhs->loc);
            return false;
        }
        if (rhs->type == DTYPE_VOID || rhs->type == DTYPE_FUNC_PTR) {
            context->printError("invalid conversion from '" + rhs->getType() + "' to '" + lhs->getType() + "'", rhs->loc);
            return false;
        }

        type = lhs->type;
        reference = lhs->reference;
        isConst = lhs->isConst;

        return true;
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + "(" + lhs->toString() + " = " + rhs->toString() + ")";
    }
    
    virtual string generateQuad(GenerationContext* generationContext) {
		// TODO: What about the quad of the lhs ?
        string ret = "";
		
		ret += rhs->generateQuad(generationContext);
		
		ret += Utils::dtypeConvQuad(rhs->type, type);
        ret += "POP " + lhs->reference->identifier + "\n";
		
		return ret;
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

        if (lhs->type == DTYPE_VOID || lhs->type == DTYPE_FUNC_PTR ||
            rhs->type == DTYPE_VOID || rhs->type == DTYPE_FUNC_PTR ||
            Utils::isBitwiseOpr(opr) && (lhs->type == DTYPE_FLOAT || rhs->type == DTYPE_FLOAT)) {
            context->printError("invalid operands of types '" + lhs->getType() + "' and '" + rhs->getType() + "' to " + getOpr(), loc);
            return false;
        }

        if (Utils::isLogicalOpr(opr)) {
            type == DTYPE_BOOL;
        } else {
            type = max(lhs->type, rhs->type);
        }

        isConst = (lhs->isConst && rhs->isConst);

        return true;
    }

    virtual string getOpr() {
        return "binary operator '" + Utils::oprToStr(opr) + "'";
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + "(" + lhs->toString() + " " + Utils::oprToStr(opr) + " " + rhs->toString() + ")";
    }
    
    virtual string generateQuad(GenerationContext* generationContext) {
		string ret = "";
		
        ret += lhs->generateQuad(generationContext);
		ret += Utils::dtypeConvQuad(lhs->type, type);
		
        ret += rhs->generateQuad(generationContext);
		ret += Utils::dtypeConvQuad(rhs->type, type);
        ret += Utils::binOprToQuad(opr) + "\n";
		
		return ret;
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

        if (expr->type == DTYPE_VOID || expr ->type == DTYPE_FUNC_PTR ||
            expr->type == DTYPE_FLOAT && Utils::isBitwiseOpr(opr)) {
            context->printError("invalid operand of type '" + expr->getType() + "' to " + getOpr(), loc);
            return false;
        }

        if (Utils::isLvalueOpr(opr)) {
            if (expr->reference == NULL) {
                context->printError("lvalue required as an operand of increment/decrement operator", expr->loc);
                return false;
            }
            if (expr->reference && expr->isConst) {
                context->printError("increment/decrement of read-only variable '" + expr->reference->header() + "'", expr->loc);
                return false;
            }
        }

        if (Utils::isLogicalOpr(opr)) {
            type == DTYPE_BOOL;
        } else {
            type = expr->type;
        }
        
        reference = expr->reference;
        isConst = expr->isConst;

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
    
    virtual string generateQuad(GenerationContext* generationContext) {
		string ret = "";
		
        switch (opr) {
            case OPR_U_MINUS:
                ret += expr->generateQuad(generationContext);
                ret += "NEG\n";
                break;
            case OPR_PRE_INC:
                ret += expr->generateQuad(generationContext);
                ret += "PUSH 1\n";
                ret += "ADD\n";
				ret += "POP " + expr->reference->identifier + "\n";
				ret += "PUSH " + expr->reference->identifier + "\n";
                break;
            case OPR_SUF_INC:
                ret += expr->generateQuad(generationContext);
				ret += "POP " + expr->reference->identifier + "\n";
				ret += "PUSH " + expr->reference->identifier + "\n";
				ret += "PUSH " + expr->reference->identifier + "\n";
                ret += "PUSH 1\n";
                ret += "ADD\n";
				ret += "POP " + expr->reference->identifier + "\n";
                break;
            case OPR_PRE_DEC:
                expr->generateQuad(generationContext);
                ret += "PUSH 1\n";
                ret += "SUB\n";
				ret += "POP " + expr->reference->identifier + "\n";
				ret += "PUSH " + expr->reference->identifier + "\n";
                break;
            case OPR_SUF_DEC:
                expr->generateQuad(generationContext);
				ret += "POP " + expr->reference->identifier + "\n";
				ret += "PUSH " + expr->reference->identifier + "\n";
				ret += "PUSH " + expr->reference->identifier + "\n";
                ret += "PUSH 1\n";
                ret += "SUB\n";
				ret += "POP " + expr->reference->identifier + "\n";
                break;
        }
		
		return ret;
    }
    
};

#endif