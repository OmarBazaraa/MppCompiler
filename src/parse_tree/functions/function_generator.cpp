#include "../parse_tree.h"
#include "../../context/generation_context.h"


string FunctionNode::generateQuad(GenerationContext* generationContext) {
    string ret;

    ret += "PROC " + alias + "\n";
    generationContext->declareFuncParams = true;

    for (int i = 0; i < paramList.size(); ++i) {
        ret += paramList[i]->generateQuad(generationContext);
    }

    generationContext->declareFuncParams = false;
    ret += body->generateQuad(generationContext);
    ret += "ENDP " + alias + "\n";

    return ret;
}

string FunctionCallNode::generateQuad(GenerationContext* generationContext) {
    string ret;

    for (int i = (int) argList.size() - 1; i >= 0; --i) {
        ret += argList[i]->generateQuad(generationContext);
        ret += Utils::dtypeConvQuad(argList[i]->type, func->paramList[i]->type->type);
    }

    ret += "CALL " + func->alias + "\n";

    return ret;
}

string ReturnStmtNode::generateQuad(GenerationContext* generationContext) {
    string ret;

    if (value) {
        ret += value->generateQuad(generationContext);
        ret += Utils::dtypeConvQuad(value->type, func->type->type);
    }

    ret += "RET\n";

    return ret;
}