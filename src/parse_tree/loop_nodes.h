#ifndef __LOOP_NODES_H_
#define __LOOP_NODES_H_

#include "../context/generation_context.h"
#include "basic_nodes.h"
#include "statement_nodes.h"


// @OmarBazaraa: there is a special case in all branch statements (i.e. if, while, do while, for):
// @OmarBazaraa: if the condition is just a value, then nothing changes the flag register, so you cannot
// @OmarBazaraa: directly use "JZ".
// @OmarBazaraa: even if there is a condition (i < n), the condition expression pops both "i" and "n",
// @OmarBazaraa: do the comparision and add the result back to the stack.
// @OmarBazaraa: so it may be reasonable to add a new instruction "TEST" that pops the last element in
// @OmarBazaraa: the stack and change the flag register in accordance.


/**
 * The node class holding a while loop in the parse tree.
 */
struct WhileNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* body;

    WhileNode(const Location& loc, ExpressionNode* cond, StatementNode* body) : StatementNode(loc) {
        this->cond = cond;
        this->body = body;
    }

    virtual ~WhileNode() {
        if (cond) delete cond;
        if (body) delete body;
    }

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("while-statement is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_LOOP);

        ret &= cond->analyze(context);
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "while (" + cond->toString() + ") \n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
        return ret;
    }

    virtual string generateQuad(GenerationContext* generationContext) {
        string ret = "";
        int label1 = generationContext->labelCounter++;
        int label2 = generationContext->labelCounter++;

        ret += "L" + to_string(label1) + ":\n";
        ret += cond->generateQuad(generationContext);
        ret += "JZ L" + to_string(label2) + "\n";

        generationContext->breakLabels.push(label2);
        generationContext->continueLabels.push(label1);

        ret += body->generateQuad(generationContext);

        generationContext->breakLabels.pop();
        generationContext->continueLabels.pop();

        ret += "JMP L" + to_string(label1) + "\n";
        ret += "L" + to_string(label2) + ":\n";

        return ret;
    }

};

/**
 * The node class holding a do-while loop in the parse tree.
 */
struct DoWhileNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* body;

    DoWhileNode(const Location& loc, ExpressionNode* cond, StatementNode* body) : StatementNode(loc) {
        this->cond = cond;
        this->body = body;
    }

    virtual ~DoWhileNode() {
        if (cond) delete cond;
        if (body) delete body;
    }

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("do-while-statement is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_LOOP);

        ret &= cond->analyze(context);
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "do\n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4)) + "\n";
        ret += string(ind, ' ') + "while (" + cond->toString() + ");";
        return ret;
    }

    virtual string generateQuad(GenerationContext* generationContext) {
        string ret = "";
        int label1 = generationContext->labelCounter++;
        int label2 = generationContext->labelCounter++;
        int label3 = generationContext->labelCounter++;

        ret += "L" + to_string(label1) + ":\n";

        generationContext->breakLabels.push(label3);
        generationContext->continueLabels.push(label2);

        ret += body->generateQuad(generationContext);

        generationContext->breakLabels.pop();
        generationContext->continueLabels.pop();

        ret += "L" + to_string(label2) + ":\n";
        ret += cond->generateQuad(generationContext);
        ret += "JNZ L" + to_string(label1) + "\n";
        ret += "L" + to_string(label3) + ":\n";

        return ret;
    }

};

/**
 * The node class holding a for loop in the parse tree.
 */
struct ForNode : public StatementNode {
    StatementNode* initStmt;
    ExpressionNode* cond;
    ExpressionNode* inc;
    StatementNode* body;

    ForNode(const Location& loc, StatementNode* initStmt, ExpressionNode* cond, ExpressionNode* inc, StatementNode* body) : StatementNode(loc) {
        this->initStmt = initStmt;
        this->cond = cond;
        this->inc = inc;
        this->body = body;
    }

    virtual ~ForNode() {
        if (initStmt) delete initStmt;
        if (cond) delete cond;
        if (inc) delete inc;
        if (body) delete body;
    }

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("for-statement is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_LOOP);

        if (initStmt) ret &= initStmt->analyze(context);
        if (cond) ret &= cond->analyze(context);
        if (inc) ret &= inc->analyze(context);
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "for (";
        ret += (initStmt ? initStmt->toString() : "") + ";";
        ret += (cond ? cond->toString() : "") + ";";
        ret += (inc ? inc->toString() : "") + ";";
        ret += ")\n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
        return ret;
    }
    /**
     * InitStmt Code
     * L1: Cond Code
     * JMP L4
     *
     * L2: Inc. Code
     * JMP L1
     *
     * L3: Body Code
     * JMP L2
     *
     * L4: JMP L5 if Condition is false
     * JMP L3
     *
     * L5 (exit)
     **/
    virtual string generateQuad(GenerationContext* generationContext) {
        string ret = "";
        int label1 = generationContext->labelCounter++;
        int label2 = generationContext->labelCounter++;
        int label3 = generationContext->labelCounter++;
        int label4 = generationContext->labelCounter++;
        int label5 = generationContext->labelCounter++;

        // @OmarBazaraa: code generation of the for loop is somehow confusing
        // @OmarBazaraa: I think it is better to do something similar to the while loop.
        // @OmarBazaraa: convert the for loop into the following...
        /*
        initStmt;
        while (cond) {
            body;
            inc;
        }
        */
        // @OmarBazaraa: the generated quadruples for the while loop is much more readable.

        if (initStmt)
            ret += initStmt->generateQuad(generationContext);

        ret += "L" + to_string(label1) + ":\n";

        if (cond)
            ret += cond->generateQuad(generationContext);

        ret += "JMP L" + to_string(label4) + "\n";
        ret += "L" + to_string(label2) + ":\n";

        if (inc)
            ret += inc->generateQuad(generationContext);

        ret += "JMP L" + to_string(label1) + "\n";
        ret += "L" + to_string(label3) + ":\n";

        generationContext->breakLabels.push(label5);
        generationContext->continueLabels.push(label2);

        ret += body->generateQuad(generationContext);

        generationContext->breakLabels.pop();
        generationContext->continueLabels.pop();

        ret += "JMP L" + to_string(label2) + "\n";
        ret += "L" + to_string(label4) + ":\n";
        ret += "JZ L" + to_string(label5) + "\n";
        ret += "JMP L" + to_string(label3) + "\n";
        ret += "L" + to_string(label5) + ":\n";

        return ret;
    }
};

#endif