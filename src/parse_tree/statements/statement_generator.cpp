#include "../parse_tree.h"
#include "../../context/generation_context.h"


string BlockNode::generateQuad(GenerationContext* context) {
    string ret;

    for (int i = 0; i < statements.size(); ++i) {
        ret += statements[i]->generateQuad(context);
    }
    
    return ret;
}

string VarDeclarationNode::generateQuad(GenerationContext* context) {
    string ret;

    if (value) {
        ret += value->generateQuad(context);
        ret += Utils::dtypeConvQuad(value->type, type->type);
    }

    if (value || context->declareFuncParams) {
        ret += Utils::oprToQuad(OPR_POP, type->type) + " " + alias + "\n";
    }

    return ret;
}

string MultiVarDeclarationNode::generateQuad(GenerationContext* context) {
    string ret;

    for (int i = 0; i < vars.size(); ++i) {
        ret += vars[i]->generateQuad(context);
    }

    return ret;
}