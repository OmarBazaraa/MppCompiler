#include "../parse_tree.h"
#include "../../context/scope_context.h"


bool ExprContainerNode::analyze(ScopeContext* context, bool valueUsed) {
    if (!context->initializeVar && context->isGlobalScope()) {
        context->log("expression is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = expr->analyze(context, valueUsed);

    type = expr->type;
    reference = expr->reference;
    constant = expr->constant;
    used = valueUsed;

    return ret;
}

bool AssignOprNode::analyze(ScopeContext* context, bool valueUsed) {
    if (!(rhs->analyze(context, true) & lhs->analyze(context, false))) {
        // Note that I used a bitwise AND to execute both lhs and rhs expressions
        return false;
    }

    if (lhs->type == DTYPE_FUNC_PTR) {
        context->log("assignment of function '" + lhs->reference->declaredHeader() + "'", lhs->loc, LOG_ERROR);
        return false;
    }
    if (lhs->reference == NULL) {
        context->log("lvalue required as left operand of assignment", lhs->loc, LOG_ERROR);
        return false;
    }
    if (lhs->reference && lhs->constant) {
        context->log("assignment of read-only variable '" + lhs->reference->declaredHeader() + "'", lhs->loc, LOG_ERROR);
        return false;
    }
    if (rhs->type == DTYPE_VOID || rhs->type == DTYPE_FUNC_PTR) {
        context->log("invalid conversion from '" + rhs->exprTypeStr() + "' to '" + lhs->exprTypeStr() + "'", rhs->loc, LOG_ERROR);
        return false;
    }

    type = lhs->type;
    reference = lhs->reference;
    constant = lhs->constant;
    used = valueUsed;

    reference->initialized = true;

    return true;
}

bool BinaryOprNode::analyze(ScopeContext* context, bool valueUsed) {
    if (!(lhs->analyze(context, valueUsed) & rhs->analyze(context, valueUsed))) {
        // Note that I used a bitwise AND to execute both lhs and rhs expressions
        return false;
    }

    if (lhs->type == DTYPE_VOID || lhs->type == DTYPE_FUNC_PTR ||
        rhs->type == DTYPE_VOID || rhs->type == DTYPE_FUNC_PTR ||
        (Utils::isBitwiseOpr(opr) || opr == OPR_MOD) && (lhs->type == DTYPE_FLOAT || rhs->type == DTYPE_FLOAT)) {
        context->log("invalid operands of types '" + lhs->exprTypeStr() + "' and '" + rhs->exprTypeStr() + "' to " +
                     getOpr(), loc, LOG_ERROR);
        return false;
    }

    if (Utils::isLogicalOpr(opr)) {
        type = DTYPE_BOOL;
    } else {
        type = max(lhs->type, rhs->type);
    }

    constant = (lhs->constant && rhs->constant);
    used = valueUsed;

    return true;
}

bool UnaryOprNode::analyze(ScopeContext* context, bool valueUsed) {
    if (!expr->analyze(context, valueUsed || Utils::isLvalueOpr(opr))) {
        return false;
    }

    if (expr->type == DTYPE_VOID || expr ->type == DTYPE_FUNC_PTR ||
        expr->type == DTYPE_FLOAT && Utils::isBitwiseOpr(opr)) {
        context->log("invalid operand of type '" + expr->exprTypeStr() + "' to " + getOpr(), loc, LOG_ERROR);
        return false;
    }

    if (Utils::isLvalueOpr(opr)) {
        if (expr->reference == NULL) {
            context->log("lvalue required as an operand of increment/decrement operator", expr->loc, LOG_ERROR);
            return false;
        }
        if (expr->reference && expr->constant) {
            context->log("increment/decrement of read-only variable '" + expr->reference->declaredHeader() + "'",
                         expr->loc, LOG_ERROR);
            return false;
        }
    }

    type = (Utils::isLogicalOpr(opr) ? DTYPE_BOOL : expr->type);
    reference = (opr == OPR_PRE_INC || opr == OPR_PRE_DEC ? expr->reference : NULL);
    constant = expr->constant;
    used = valueUsed;

    return true;
}

bool IdentifierNode::analyze(ScopeContext* context, bool valueUsed) {
    DeclarationNode* ptr = context->getSymbol(name);

    if (ptr == NULL) {
        context->log("'" + name + "' was not declared in this scope", loc, LOG_ERROR);
        return false;
    }

    reference = ptr;

    if (dynamic_cast<FunctionNode*>(ptr)) {
        type = DTYPE_FUNC_PTR;
    } else {
        type = ptr->type->type;
        constant = ((VarDeclarationNode*) ptr)->constant;
    }

    used = valueUsed;

    if (used) {
        reference->used++;
    }

    if (used && !reference->initialized) {
        context->log("variable or field '" + name + "' used without being initialized", loc, LOG_ERROR);
        return false;
    }

    return true;
}
