#include "../parse_tree.h"
#include "../../context/generation_context.h"


string IfNode::generateQuad(GenerationContext* generationContext) {
    string ret;
    int label1 = generationContext->labelCounter++;
    
    ret += cond->generateQuad(generationContext);
    ret += Utils::oprToQuad(OPR_JZ, cond->type) + " L" + to_string(label1) + "\n";
    ret += ifBody->generateQuad(generationContext);
    
    if (elseBody) {
        int label2 = generationContext->labelCounter++;

        ret += Utils::oprToQuad(OPR_JMP) + " L" + to_string(label2) + "\n";
        ret += "L" + to_string(label1) + ":\n";
        ret += elseBody->generateQuad(generationContext);
        ret += "L" + to_string(label2) + ":\n";
    }
    else {
        ret += "L" + to_string(label1) + ":\n";
    }

    return ret;
}

string CaseLabelNode::generateQuad(GenerationContext* generationContext) {
    if (expr == NULL) {
        return stmt->generateQuad(generationContext);
    }

    string ret;
    int label1 = generationContext->labelCounter++;

    // @AbdoEed The type of this push is incorrect it should be the type of the switch expr. not the case expr.
    ret += Utils::oprToQuad(OPR_PUSH, expr->type) + " SWITCH_COND@" + to_string(generationContext->breakLabels.top()) + "\n";
    ret += expr->generateQuad(generationContext);
    ret += Utils::oprToQuad(OPR_EQUAL, expr->type);
    ret += Utils::oprToQuad(OPR_JZ, DTYPE_BOOL) + " L" + to_string(label1) + "\n";
    ret += stmt->generateQuad(generationContext);
    ret += "L" + to_string(label1) + ":\n";

    return ret;
}

string SwitchNode::generateQuad(GenerationContext* generationContext) {
    string ret;
    int label1 = generationContext->labelCounter++;

    ret += cond->generateQuad(generationContext);
    ret += Utils::oprToQuad(OPR_POP, cond->type) + " SWITCH_COND@" + to_string(label1) + "\n";
    generationContext->breakLabels.push(label1);

    ret += body->generateQuad(generationContext);

    generationContext->breakLabels.pop();
    ret += "L" + to_string(label1) + ":\n";

    return ret;
}

string WhileNode::generateQuad(GenerationContext* generationContext) {
    string ret;
    int label1 = generationContext->labelCounter++;
    int label2 = generationContext->labelCounter++;

    ret += "L" + to_string(label1) + ":\n";
    ret += cond->generateQuad(generationContext);
    ret += Utils::oprToQuad(OPR_JZ, cond->type) + " L" + to_string(label2) + "\n";

    generationContext->breakLabels.push(label2);
    generationContext->continueLabels.push(label1);

    ret += body->generateQuad(generationContext);

    generationContext->breakLabels.pop();
    generationContext->continueLabels.pop();

    ret += Utils::oprToQuad(OPR_JMP) + " L" + to_string(label1) + "\n";
    ret += "L" + to_string(label2) + ":\n";

    return ret;
}

string DoWhileNode::generateQuad(GenerationContext* generationContext) {
    string ret;
    int label1 = generationContext->labelCounter++;
    int label2 = generationContext->labelCounter++;
    int label3 = generationContext->labelCounter++;

    ret += "L" + to_string(label1) + ":\n";

    generationContext->breakLabels.push(label3);
    generationContext->continueLabels.push(label2);

    ret += body->generateQuad(generationContext);

    generationContext->continueLabels.pop();
    generationContext->breakLabels.pop();

    ret += "L" + to_string(label2) + ":\n";
    ret += cond->generateQuad(generationContext);
    ret += Utils::oprToQuad(OPR_JNZ, cond->type) + " L" + to_string(label1) + "\n";
    ret += "L" + to_string(label3) + ":\n";

    return ret;
}

string ForNode::generateQuad(GenerationContext* generationContext) {
    /**
     * InitStmt Code
     * L1: Cond Code
     * JMP L3 if Condition is false
     *
     * Body Code
     * L2: Inc. Code
     * JMP L1
     *
     * L3 (exit)
     *
     **/

    string ret;
    int label1 = generationContext->labelCounter++;
    int label2 = generationContext->labelCounter++;
    int label3 = generationContext->labelCounter++;

    if (initStmt) {
        ret += initStmt->generateQuad(generationContext);
    }

    ret += "L" + to_string(label1) + ":\n";

    if (cond) {
        ret += cond->generateQuad(generationContext);
        ret += Utils::oprToQuad(OPR_JZ, cond->type) + " L" + to_string(label3) + "\n";
    }
    
    generationContext->breakLabels.push(label3);
    generationContext->continueLabels.push(label2);

    ret += body->generateQuad(generationContext);

    generationContext->continueLabels.pop();
    generationContext->breakLabels.pop();
    
    ret += "L" + to_string(label2) + ":\n";
    
    if (inc) {
        ret += inc->generateQuad(generationContext);
    }
        
    ret += Utils::oprToQuad(OPR_JMP) + " L" + to_string(label1) + "\n";
    ret += "L" + to_string(label3) + ":\n";

    return ret;
}

string BreakStmtNode::generateQuad(GenerationContext* generationContext) {
    return Utils::oprToQuad(OPR_JMP) + " L" + to_string(generationContext->breakLabels.top()) + "\n";
}

string ContinueStmtNode::generateQuad(GenerationContext* generationContext) {
    return Utils::oprToQuad(OPR_JMP) + " L" + to_string(generationContext->continueLabels.top()) + "\n";
}