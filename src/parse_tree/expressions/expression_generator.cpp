#include "../parse_tree.h"
#include "../../context/generation_context.h"


string ExprContainerNode::generateQuad(GenerationContext* context) {
    return expr->generateQuad(context);
}

string AssignOprNode::generateQuad(GenerationContext* context) {
    string ret;

    ret += lhs->generateQuad(context);
    ret += rhs->generateQuad(context);
    ret += Utils::dtypeConvQuad(rhs->type, type);
    ret += Utils::oprToQuad(OPR_POP, type) + " " + lhs->reference->alias + "\n";

    if (used) {
        ret += Utils::oprToQuad(OPR_PUSH, type) + " " + lhs->reference->alias + "\n";
    }

    return ret;
}

string BinaryOprNode::generateQuad(GenerationContext* context) {
    string ret;
    
    DataType t = max(lhs->type, rhs->type);

    if (used) {
        ret += lhs->generateQuad(context);
        ret += Utils::dtypeConvQuad(lhs->type, t);

        ret += rhs->generateQuad(context);
        ret += Utils::dtypeConvQuad(rhs->type, t);

        ret += Utils::oprToQuad(opr, t) + "\n";
    }
    else {
        ret += lhs->generateQuad(context);
        ret += rhs->generateQuad(context);
    }

    return ret;
}

string UnaryOprNode::generateQuad(GenerationContext* context) {
    string ret;
    
    ret += expr->generateQuad(context);

    if (used) {
        ret += Utils::dtypeConvQuad(expr->type, type);
    }

    switch (opr) {
        case OPR_PRE_INC:
        case OPR_PRE_DEC:
            ret += Utils::oprToQuad(opr, type) + "\n";
            ret += Utils::oprToQuad(OPR_POP, type) + " " + expr->reference->alias + "\n";

            if (used) {
                ret += Utils::oprToQuad(OPR_PUSH, type) + " " + expr->reference->alias + "\n";
            }
            break;
        case OPR_SUF_INC:
        case OPR_SUF_DEC:
            if (used) {
                ret += Utils::oprToQuad(OPR_PUSH, type) + " " + expr->reference->alias + "\n";
            }
            
            ret += Utils::oprToQuad(opr, type) + "\n";
            ret += Utils::oprToQuad(OPR_POP, type) + " " + expr->reference->alias + "\n";
            break;
        case OPR_U_MINUS:
        case OPR_NOT:
        case OPR_LOGICAL_NOT:
            if (used) {
                ret += Utils::oprToQuad(opr, type) + "\n";
            }
            break;
    }

    return ret;
}

string IdentifierNode::generateQuad(GenerationContext* context) {
    string ret;
    if (used) {
        ret += Utils::oprToQuad(OPR_PUSH, type) + " " + reference->alias + "\n";;
    }
    return ret;
}

string ValueNode::generateQuad(GenerationContext* context) {
    string ret;
    if (used) {
        ret += Utils::oprToQuad(OPR_PUSH, type) + " " + value + "\n";
    }
    return ret;
}