#include "../parse_tree.h"
#include "../../context/scope_context.h"


int BinaryOprNode::getConstIntValue() {
    int l = lhs->getConstIntValue();
    int r = rhs->getConstIntValue();

    switch (opr) {
        case OPR_ADD:
            return l + r;
        case OPR_SUB:
            return l - r;
        case OPR_MUL:
            return l * r;
        case OPR_DIV:
            return l / r;
        case OPR_MOD:
            return l % r;
        case OPR_AND:
            return l & r;
        case OPR_OR:
            return l | r;
        case OPR_XOR:
            return l ^ r;
        case OPR_SHL:
            return l << r;
        case OPR_SHR:
            return l >> r;
        case OPR_LOGICAL_AND:
            return l && r;
        case OPR_LOGICAL_OR:
            return l || r;
        case OPR_GREATER:
            return l > r;
        case OPR_GREATER_EQUAL:
            return l >= r;
        case OPR_LESS:
            return l < r;
        case OPR_LESS_EQUAL:
            return l <= r;
        case OPR_EQUAL:
            return l == r;
        case OPR_NOT_EQUAL:
            return l != r;
    }

    return -1;
}

int UnaryOprNode::getConstIntValue() {
    int v = expr->getConstIntValue();

    switch (opr) {
        case OPR_U_PLUS:
            return v;
        case OPR_U_MINUS:
            return -v;
        case OPR_NOT:
            return ~v;
        case OPR_LOGICAL_NOT:
            return !v;
    }

    return -1;
}

int IdentifierNode::getConstIntValue() {
    VarDeclarationNode* var = dynamic_cast<VarDeclarationNode*>(reference);

    if (Utils::isIntegerType(type) && var != NULL && var->initialized) {
        return var->value->getConstIntValue();
    }

    return -1;
}

int ValueNode::getConstIntValue() {
    switch (type) {
        case DTYPE_BOOL:
            return value == "true";
        case DTYPE_CHAR:
            return value[1];
        case DTYPE_INT:
            return atoi(value.c_str());
    }

    return -1;
}
