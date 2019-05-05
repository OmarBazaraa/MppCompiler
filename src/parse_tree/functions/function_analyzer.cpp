#include "../parse_tree.h"
#include "../../context/scope_context.h"


bool FunctionNode::analyze(ScopeContext* context) {
    if (!context->isGlobalScope()) {
        context->printError("a function-definition is not allowed here", ident->loc);
        return false;
    }

    bool ret = true;

    if (!context->declareSymbol(this)) {
        context->printError("'" + declaredHeader() + "' redeclared", ident->loc);
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
        context->printError("'" + ident->name + "' was not declared in this scope", loc);
        ret = false;
    }
    else if (func == NULL) {
        context->printError("'" + ident->name + "' cannot be used as a function", loc);
        ret = false;
    }
    else if (argList.size() > func->paramList.size()) {
        context->printError("too many arguments to function '" + func->declaredHeader() + "'", loc);
        ret = false;
    }
    else if (argList.size() < func->paramList.size()) {
        context->printError("too few arguments to function '" + func->declaredHeader() + "'", loc);
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
            context->printError("invalid conversion from '" + argList[i]->exprTypeStr() + "' to '" +
                func->paramList[i]->declaredType() + "' in function '" +
                func->declaredHeader() + "' call", argList[i]->loc);
            return false;
        }
    }

    used = valueUsed;

    if (ret) {
        func->used = true;
    }

    return ret;
}

bool ReturnStmtNode::analyze(ScopeContext* context) {
    func = context->getFunctionScope();

    if (func == NULL) {
        context->printError("return-statement not within function", loc);
        return false;
    }

    if (value) {    // return expression exists
        if (!value->analyze(context, true)) {
            return false;
        }

        if (func->type->type == DTYPE_VOID && value->type != DTYPE_VOID) {
            context->printError("return-statement with '" + value->exprTypeStr() + "' value, in function returning 'void'", value->loc);
            return false;
        }
    }
    else {          // No return expression
        if (func->type->type != DTYPE_VOID) {
            context->printError("return-statement with no value, in function returning '" + func->type->toString() + "'", loc);
            return false;
        }
    }

    return true;
}
