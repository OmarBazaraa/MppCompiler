#include "../parse_tree.h"
#include "../../context/scope_context.h"


bool IfNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->log("if-statement is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_IF, this);

    ret &= cond->analyze(context, true);

    if (ret && cond->type == DTYPE_VOID) {
        context->log("invalid conversion from '" + cond->exprTypeStr() + "' to 'bool'", cond->loc, LOG_ERROR);
        ret = false;
    }

    ret &= ifBody->analyze(context);

    if (elseBody) {
        ret &= elseBody->analyze(context);
    }

    context->popScope();

    return ret;
}

bool CaseLabelNode::analyze(ScopeContext* context) {
    SwitchNode* switchStmt = context->getSwitchScope();

    if (switchStmt == NULL) {
        context->log("case label not within switch statement", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    if (switchStmt->initializedVars.size() > 0) {
        context->log("jump to case label", expr->loc, LOG_ERROR);
        ret = false;

        const VarList& list = switchStmt->initializedVars;

        for (int i = 0; i < list.size(); ++i) {
            context->log("crosses initialization of '" + list[i]->declaredHeader() + "'", list[i]->ident->loc, LOG_NOTE);
        }
    }

    if (expr) {     // case label
        ret = expr->analyze(context, true);

        if (ret && !expr->constant) {
            context->log("constant expression required in case label", expr->loc, LOG_ERROR);
            ret = false;
        }
        if (ret && !Utils::isIntegerType(expr->type)) {
            context->log("case quantity not an integer", expr->loc, LOG_ERROR);
            ret = false;
        }
        if (ret && expr->constant && Utils::isIntegerType(expr->type)) {
            int val = expr->getConstIntValue();

            if (switchStmt->caseMap.count(val)) {
                context->log("duplicate case value", loc, LOG_ERROR);
                context->log("previously used here", switchStmt->caseMap[val]->loc, LOG_NOTE);
                ret = false;
            }

            switchStmt->caseMap[val] = this;
        }
    }
    else {          // default label
        if (switchStmt->hasDefaultLabel) {
            context->log("multiple default labels in one switch", loc, LOG_ERROR);
            ret = false;
        }

        switchStmt->hasDefaultLabel = true;
    }

    ret &= stmt->analyze(context);

    return ret;
}

bool SwitchNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->log("switch-statement is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    populate(); // Pre-compute switch cases and statements

    context->addScope(SCOPE_SWITCH, this);

    ret &= cond->analyze(context, true);

    if (!Utils::isIntegerType(cond->type)) {
        context->log("switch quantity not an integer", cond->loc, LOG_ERROR);
        ret = false;
    }

    ret &= body->analyze(context);

    context->popScope();

    return ret;
}

bool WhileNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->log("while-statement is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_LOOP, this);

    ret &= cond->analyze(context, true);

    if (ret && cond->type == DTYPE_VOID) {
        context->log("invalid conversion from '" + cond->exprTypeStr() + "' to 'bool'", cond->loc, LOG_ERROR);
        ret = false;
    }

    ret &= body->analyze(context);

    context->popScope();

    return ret;
}

bool DoWhileNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->log("do-while-statement is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_LOOP, this);

    ret &= cond->analyze(context, true);

    if (ret && cond->type == DTYPE_VOID) {
        context->log("invalid conversion from '" + cond->exprTypeStr() + "' to 'bool'", cond->loc, LOG_ERROR);
        ret = false;
    }

    ret &= body->analyze(context);

    context->popScope();

    return ret;
}

bool ForNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->log("for-statement is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_LOOP, this);

    if (initStmt) {
        ret &= initStmt->analyze(context);
    }

    if (cond) {
        ret &= cond->analyze(context, true);

        if (ret && cond->type == DTYPE_VOID) {
            context->log("invalid conversion from '" + cond->exprTypeStr() + "' to 'bool'", cond->loc, LOG_ERROR);
            ret = false;
        }
    }

    if (inc) {
        ret &= inc->analyze(context, false);
    }

    ret &= body->analyze(context);

    context->popScope();

    return ret;
}

bool BreakStmtNode::analyze(ScopeContext* context) {
    if (!context->hasBreakScope()) {
        context->log("break-statement not within loop or switch", loc, LOG_ERROR);
        return false;
    }

    return true;
}

bool ContinueStmtNode::analyze(ScopeContext* context) {
    if (!context->hasLoopScope()) {
        context->log("continue-statement not within loop", loc, LOG_ERROR);
        return false;
    }

    return true;
}
