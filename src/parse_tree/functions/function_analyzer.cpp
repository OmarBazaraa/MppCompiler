#include "../parse_tree.h"
#include "../../context/scope_context.h"


bool FunctionNode::analyze(ScopeContext* context) {
    if (!context->isGlobalScope()) {
        context->log("a function-definition is not allowed here", ident->loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    if (!context->declareSymbol(this)) {
        context->log("'" + declaredHeader() + "' redeclared", ident->loc, LOG_ERROR);
        ret = false;
    }

    context->addScope(SCOPE_FUNCTION, this);

    context->declareFuncParams = true;
    for (int i = 0; i < paramList.size(); ++i) {
        ret &= paramList[i]->analyze(context);
    }
    context->declareFuncParams = false;

    ret &= body->analyze(context);

    context->popScope();

    return ret;
}

bool FunctionCallNode::analyze(ScopeContext* context, bool valueUsed) {
    bool ret = true;

    DeclarationNode* ptr = context->getSymbol(ident->name);
    func = dynamic_cast<FunctionNode*>(ptr);

    if (ptr == NULL) {
        context->log("'" + ident->name + "' was not declared in this scope", loc, LOG_ERROR);
        ret = false;
    }
    else if (func == NULL) {
        context->log("'" + ident->name + "' cannot be used as a function", loc, LOG_ERROR);
        ret = false;
    }
    else if (argList.size() > func->paramList.size()) {
        context->log("too many arguments to function '" + func->declaredHeader() + "'", loc, LOG_ERROR);
        ret = false;
    }
    else if (argList.size() < func->paramList.size()) {
        context->log("too few arguments to function '" + func->declaredHeader() + "'", loc, LOG_ERROR);
        ret = false;
    } else {
        type = ptr->type->type;
    }

    for (int i = 0; i < argList.size(); ++i) {
        if (!argList[i]->analyze(context, true)) {
            ret = false;
            continue;
        }

        if (func && argList[i]->type == DTYPE_VOID || argList[i]->type == DTYPE_FUNC_PTR) {
            context->log("invalid conversion from '" + argList[i]->exprTypeStr() + "' to '" +
                         func->paramList[i]->type->toString() + "' in function '" +
                         func->declaredHeader() + "' call", argList[i]->loc, LOG_ERROR);
            return false;
        }
    }

    used = valueUsed;

    if (ret) {
        func->used++;
    }

    return ret;
}

bool ReturnStmtNode::analyze(ScopeContext* context) {
    func = context->getFunctionScope();

    if (func == NULL) {
        context->log("return-statement not within function", loc, LOG_ERROR);
        return false;
    }

    if (value) {    // return expression exists
        if (!value->analyze(context, true)) {
            return false;
        }

        if (func->type->type == DTYPE_VOID && value->type != DTYPE_VOID) {
            context->log("return-statement with '" + value->exprTypeStr() + "' value, in function returning 'void'",
                         value->loc, LOG_ERROR);
            return false;
        }
    }
    else {          // No return expression
        if (func->type->type != DTYPE_VOID) {
            context->log("return-statement with no value, in function returning '" + func->type->toString() + "'", loc, LOG_ERROR);
            return false;
        }
    }

    return true;
}
