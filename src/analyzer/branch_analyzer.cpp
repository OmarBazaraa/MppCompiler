#include "../symbol_table/symbol_table.h"
#include "../context/scope_context.h"
#include "../parse_tree/parse_tree.h"


bool IfNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->printError("if-statement is not allowed in global scope", loc);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_IF);

    ret &= cond->analyze(context, true);
    ret &= ifBody->analyze(context);

    if (elseBody) {
        ret &= elseBody->analyze(context);
    }

    context->popScope();

    return ret;
}

bool CaseLabelNode::analyze(ScopeContext* context) {
    Switch* switchStmt = context->switches.empty() ? NULL : context->switches.top();

    if (switchStmt == NULL) {
        context->printError("case label not within switch statement", loc);
        return false;
    }

    bool ret = true;

    if (expr) {     // case label
        ret = expr->analyze(context, true);

        if (ret && !expr->isConst) {
            context->printError("constant expression required in case label", expr->loc);
            ret = false;
        }
        if (ret && !Utils::isIntegerType(expr->type)) {
            context->printError("case quantity not an integer", expr->loc);
            ret = false;
        }
        if (ret && expr->isConst && Utils::isIntegerType(expr->type)) {
            // TODO: calculate this value
            // int val = 0;

            // if (switchStmt->labels.count(val)) {
            //     context->printError("duplicate case value", loc);
            //     ret = false;
            // }

            // switchStmt->labels.insert(val);
        }
    }
    else {          // default label
        if (switchStmt->defaultLabel) {
            context->printError("multiple default labels in one switch", loc);
            ret = false;
        }

        switchStmt->defaultLabel = true;
    }

    ret &= stmt->analyze(context);

    return ret;
}

bool SwitchNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->printError("switch-statement is not allowed in global scope", loc);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_SWITCH);
    context->switches.push(&switchStmt);

    ret &= cond->analyze(context, true);

    if (!Utils::isIntegerType(cond->type)) {
        context->printError("switch quantity not an integer", cond->loc);
        ret = false;
    }

    ret &= body->analyze(context);

    context->switches.pop();
    context->popScope();

    return ret;
}

bool WhileNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->printError("while-statement is not allowed in global scope", loc);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_LOOP);

    ret &= cond->analyze(context, true);
    ret &= body->analyze(context);

    context->popScope();

    return ret;
}

bool DoWhileNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->printError("do-while-statement is not allowed in global scope", loc);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_LOOP);

    ret &= cond->analyze(context, true);
    ret &= body->analyze(context);

    context->popScope();

    return ret;
}

bool ForNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->printError("for-statement is not allowed in global scope", loc);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_LOOP);

    if (initStmt) ret &= initStmt->analyze(context);
    if (cond) ret &= cond->analyze(context, true);
    if (inc) ret &= inc->analyze(context, false);
    ret &= body->analyze(context);

    context->popScope();

    return ret;
}

bool BreakStmtNode::analyze(ScopeContext* context) {
    if (!context->hasBreakScope()) {
        context->printError("break-statement not within loop or switch", loc);
        return false;
    }

    return true;
}

bool ContinueStmtNode::analyze(ScopeContext* context) {
    if (!context->hasLoopScope()) {
        context->printError("continue-statement not within loop", loc);
        return false;
    }

    return true;
}
