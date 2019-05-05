#include "../symbol_table/symbol_table.h"
#include "../context/scope_context.h"
#include "../parse_tree/parse_tree.h"


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

    context->addScope(SCOPE_BLOCK);

    for (int i = 0; i < statements.size(); ++i) {
        ret &= statements[i]->analyze(context);
    }

    context->popScope();

    return ret;
}

bool VarDeclarationNode::analyze(ScopeContext* context) {
    bool ret = true;

    if (type->type == DTYPE_VOID) {
        context->printError("variable or field '" + name->name + "' declared void", name->loc);
        ret = false;
    }
    else if (!context->declareSymbol(&var)) {
        context->printError("'" + var.header() + "' redeclared", name->loc);
        ret = false;
    }

    if (value) {
        context->initializeVar = true;
        ret &= value->analyze(context, true);
        context->initializeVar = false;
    }

    if (context->declareFuncParams && var.isInitialized) {
        context->printError("default function parameters are not allowed", value->loc);
        ret = false;
    }
    else if (var.isConst && !var.isInitialized && !context->declareFuncParams) {
        context->printError("uninitialized const '" + name->name + "'", name->loc);
        ret = false;
    }

    return ret;
}
