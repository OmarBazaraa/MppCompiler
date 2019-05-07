#include "../parse_tree.h"
#include "../../context/scope_context.h"


bool ErrorNode::analyze(ScopeContext* context) {
    context->printError(what, loc);
    return false;
}

bool BlockNode::analyze(ScopeContext* context) {
    if (context->isGlobalScope()) {
        context->printError("block is not allowed in global scope", loc);
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
        context->printError("variable or field '" + ident->name + "' declared void", ident->loc);
        ret = false;
    }
    else if (!context->declareSymbol(this)) {
        context->printError("'" + declaredHeader() + "' redeclared", ident->loc);
        ret = false;
    }

    if (value) {
        context->initializeVar = true;
        ret &= value->analyze(context, true);
        context->initializeVar = false;
    }

    if (context->declareFuncParams && value != NULL) {
        context->printError("default function parameters are not allowed", value->loc);
        ret = false;
    }
    else if (constant && value == NULL && !context->declareFuncParams) {
        context->printError("uninitialized const '" + ident->name + "'", ident->loc);
        ret = false;
    }

    SwitchNode* switchStmt = context->getSwitchScope();

    if (switchStmt != NULL && switchStmt->caseLabelCnt < switchStmt->caseLabels.size() && value != NULL) {
        context->printError("cross initialization of '" + declaredHeader() + "'", ident->loc);
        ret = false;
    }

    return ret;
}
