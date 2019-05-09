#include "../parse_tree.h"
#include "../../context/generation_context.h"


string BlockNode::generateQuad(GenerationContext* generationContext) {
    string ret;

    for (int i = 0; i < statements.size(); ++i) {
        ret += statements[i]->generateQuad(generationContext);
    }
    
    return ret;
}

string VarDeclarationNode::generateQuad(GenerationContext* generationContext) {
    string ret;

    if (value) {
        ret += value->generateQuad(generationContext);
        ret += Utils::dtypeConvQuad(value->type, type->type);
    }

    if (value || generationContext->declareFuncParams) {
        ret += Utils::oprToQuad(OPR_POP, type->type) + " " + alias + "\n";
    }

    return ret;
}

string MultiVarDeclarationNode::generateQuad(GenerationContext* generationContext) {
    string ret;

    for (int i = 0; i < vars.size(); ++i) {
        ret += vars[i]->generateQuad(generationContext);
    }

    return ret;
}