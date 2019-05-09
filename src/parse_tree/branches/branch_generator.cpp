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

string SwitchNode::generateQuad(GenerationContext* generationContext) {
    string ret;
    vector<pair<int, int>> labelPairs;
    int defaultLabel = -1;
    int breakLabel = generationContext->labelCounter++;

    ret += cond->generateQuad(generationContext);
    ret += Utils::oprToQuad(OPR_POP, cond->type) + " SWITCH_COND@" + to_string(breakLabel) + "\n";
    generationContext->breakLabels.push(breakLabel);

    for (int i = 0; i < caseLabels.size(); i++) {
        int label1 = generationContext->labelCounter++;
        if (caseLabels[i] == NULL) {
            defaultLabel = label1;
            labelPairs.push_back({-1, label1});
        }
        else {
            int label2 = generationContext->labelCounter++;
            labelPairs.push_back({label1, label2});
        }
    }

    for (int i = 0; i < caseLabels.size(); i++) {
        if (caseLabels[i]) {
            if (i > 0) {
                ret += Utils::oprToQuad(OPR_JMP) + " L" + to_string(labelPairs[i].second) + "\n";
            }

            DataType resultType = max(cond->type, caseLabels[i]->type);

            ret += "L" + to_string(labelPairs[i].first) + ":\n";
            ret += Utils::oprToQuad(OPR_PUSH, cond->type) + " SWITCH_COND@" + to_string(generationContext->breakLabels.top()) + "\n";
            ret += Utils::dtypeConvQuad(cond->type, resultType);
            ret += Utils::oprToQuad(OPR_PUSH, caseLabels[i]->type) + " " + to_string(caseLabels[i]->getConstIntValue()) + "\n";
            ret += Utils::dtypeConvQuad(caseLabels[i]->type, resultType);
            ret += Utils::oprToQuad(OPR_EQUAL, resultType) + "\n";
            ret += Utils::oprToQuad(OPR_JZ, DTYPE_BOOL) + " L";

            if (i == caseLabels.size() - 1) {               // my case label is last
                ret += (hasDefaultLabel ? to_string(defaultLabel) : to_string(breakLabel)) + "\n";
            }
            else if (labelPairs[i + 1].first == -1) {       // my next label is default
                ret += ((i + 1 == caseLabels.size() - 1) ? to_string(defaultLabel) : to_string(labelPairs[i + 2].first)) + "\n";
            }
            else {                                          // my next is case
                ret += to_string(labelPairs[i + 1].first) + "\n";
            }
        }

        ret += "L" + to_string(labelPairs[i].second) + ":\n";

        for (int j = 0;j < caseStmts[i].size();j++) {
            ret += caseStmts[i][j]->generateQuad(generationContext);
        }
    }

    generationContext->breakLabels.pop();
    ret += "L" + to_string(breakLabel) + ":\n";

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