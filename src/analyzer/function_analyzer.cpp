#include "../symbol_table/symbol_table.h"
#include "../context/scope_context.h"
#include "../parse_tree/parse_tree.h"


bool FunctionNode::analyze(ScopeContext* context) {
    if (!context->isGlobalScope()) {
        context->printError("a function-definition is not allowed here", name->loc);
        return false;
    }

    bool ret = true;

    if (!context->declareSymbol(&func)) {
        context->printError("'" + func.header() + "' redeclared", name->loc);
        ret = false;
    }

    context->addScope(SCOPE_FUNCTION);
    context->functions.push(&func);

    context->declareFuncParams = true;
    for (int i = 0; i < paramList.size(); ++i) {
        ret &= paramList[i]->analyze(context);
    }
    context->declareFuncParams = false;

    ret &= body->analyze(context);

    context->functions.pop();
    context->popScope();

    return ret;
}

bool FunctionCallNode::analyze(ScopeContext* context, bool valueUsed) {
    bool ret = true;

    Symbol* ptr = context->getSymbol(name->name);
    func = dynamic_cast<Func*>(ptr);

    if (ptr == NULL) {
        context->printError("'" + name->name + "' was not declared in this scope", loc);
        ret = false;
    }
    else if (func == NULL) {
        context->printError("'" + name->name + "' cannot be used as a function", loc);
        ret = false;
    }
    else if (argList.size() > func->paramList.size()) {
        context->printError("too many arguments to function '" + func->header() + "'", loc);
        ret = false;
    }
    else if (argList.size() < func->paramList.size()) {
        context->printError("too few arguments to function '" + func->header() + "'", loc);
        ret = false;
    } else {
        type = ptr->type;
    }

    for (int i = 0; i < argList.size(); ++i) {
        if (!argList[i]->analyze(context, true)) {
            ret = false;
            continue;
        }

        if (func && argList[i]->type == DTYPE_VOID || argList[i]->type == DTYPE_FUNC_PTR) {
            context->printError("invalid conversion from '" + argList[i]->getType() + "' to '" +
                func->paramList[i].getType() + "' in function '" +
                func->header() + "' call", argList[i]->loc);
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
    func = context->functions.empty() ? NULL : context->functions.top();

    if (func == NULL) {
        context->printError("return-statement not within function", loc);
        return false;
    }

    if (value) {    // return expression exists
        if (!value->analyze(context, true)) {
            return false;
        }

        if (func->type == DTYPE_VOID && value->type != DTYPE_VOID) {
            context->printError("return-statement with '" + value->getType() + "' value, in function returning 'void'", value->loc);
            return false;
        }
    }
    else {          // No return expression
        if (func->type != DTYPE_VOID) {
            context->printError("return-statement with no value, in function returning '" + Utils::dtypeToStr(func->type) + "'", loc);
            return false;
        }
    }

    return true;
}
