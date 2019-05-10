#include "../parse_tree.h"
#include "../../context/generation_context.h"


string FunctionNode::generateQuad(GenerationContext* context) {
    string ret;

    ret += "PROC " + alias + "\n";
    context->declareFuncParams = true;

    for (int i = 0; i < paramList.size(); ++i) {
        ret += paramList[i]->generateQuad(context);
    }

    context->declareFuncParams = false;
    ret += body->generateQuad(context);
    ret += "ENDP " + alias + "\n";

    return ret;
}

string FunctionCallNode::generateQuad(GenerationContext* context) {
    string ret;

    for (int i = (int) argList.size() - 1; i >= 0; --i) {
        ret += argList[i]->generateQuad(context);
        ret += Utils::dtypeConvQuad(argList[i]->type, func->paramList[i]->type->type);
    }

    ret += "CALL " + func->alias + "\n";

    return ret;
}

string ReturnStmtNode::generateQuad(GenerationContext* context) {
    string ret;

    if (value) {
        ret += value->generateQuad(context);
        ret += Utils::dtypeConvQuad(value->type, func->type->type);
    }

    ret += "RET\n";

    return ret;
}