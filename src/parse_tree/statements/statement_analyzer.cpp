#include "../parse_tree.h"
#include "../../context/scope_context.h"


bool ErrorNode::analyze(ScopeContext* context) {
    context->log(what, loc, LOG_ERROR);
    return false;
}

bool BlockNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->log("block is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_BLOCK, this);

    for (int i = 0; i < statements.size(); ++i) {
        ret &= statements[i]->analyze(context);
    }

    context->popScope();

    return ret;
}

bool VarDeclarationNode::analyze(ScopeContext* context) {
    bool ret = true;

    if (type->type == DTYPE_VOID) {
        context->log("variable or field '" + ident->name + "' declared void", ident->loc, LOG_ERROR);
        ret = false;
    }
    else if (!context->declareSymbol(this)) {
        context->log("'" + declaredHeader() + "' redeclared", ident->loc, LOG_ERROR);
        ret = false;
    }

    if (context->declareFuncParams) {
        initialized = true;
    }

    if (value) {
        context->initializeVar = true;
        ret &= value->analyze(context, true);
        context->initializeVar = false;
    }

    if (ret && value != NULL && (value->type == DTYPE_VOID || value->type == DTYPE_FUNC_PTR)) {
        context->log("invalid conversion from '" + value->exprTypeStr() + "' to '" + type->toString() + "'", value->loc, LOG_ERROR);
        ret = false;
    }
    else if (context->declareFuncParams && value != NULL) {
        context->log("default function parameters are not allowed", value->loc, LOG_ERROR);
        ret = false;
    }
    else if (constant && value == NULL && !context->declareFuncParams) {
        context->log("uninitialized const '" + ident->name + "'", ident->loc, LOG_ERROR);
        ret = false;
    }

    SwitchNode* switchStmt = context->getSwitchScope();

    if (switchStmt != NULL && value != NULL) {
        switchStmt->initializedVars.push_back(this);
    }

    return ret;
}

bool MultiVarDeclarationNode::analyze(ScopeContext* context) {
    int ret = true;

    for (int i = 0; i < vars.size(); ++i) {
        ret &= vars[i]->analyze(context);
    }

    return ret;
}