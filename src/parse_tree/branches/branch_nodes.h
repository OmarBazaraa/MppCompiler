#ifndef __CONTROL_NODES_H_
#define __CONTROL_NODES_H_

#include "../basic_nodes.h"
#include "../statements/statement_nodes.h"
#include "../expressions/expression_nodes.h"


/**
 * The node class holding an if statement in the parse tree.
 */
struct IfNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* ifBody;
    StatementNode* elseBody;

    IfNode(const Location& loc, ExpressionNode* cond, StatementNode* ifBody, StatementNode* elseBody = NULL)
            : StatementNode(loc) {
        this->cond = cond;
        this->ifBody = ifBody;
        this->elseBody = elseBody;
    }

    virtual ~IfNode() {
        if (cond) delete cond;
        if (ifBody) delete ifBody;
        if (elseBody) delete elseBody;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "if (" + cond->toString() + ")\n";
        ret += ifBody->toString(ind + (dynamic_cast<BlockNode*>(ifBody) ? 0 : 4));

        if (elseBody) {
            ret += "\n" + string(ind, ' ') + "else\n";
            ret += elseBody->toString(ind + (dynamic_cast<BlockNode*>(elseBody) ? 0 : 4));
        }

        return ret;
    }
};

/**
 * The node class holding a case label in the parse tree.
 */
struct CaseLabelNode : public StatementNode {
    ExpressionNode* expr;
    StatementNode* stmt;

    CaseLabelNode(const Location& loc, ExpressionNode* expr, StatementNode* stmt) : StatementNode(loc) {
        this->expr = expr;
        this->stmt = stmt;
    }

    virtual ~CaseLabelNode() {
        if (expr) delete expr;
        if (stmt) delete stmt;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(max(0, ind - 4), ' ') + (expr ? "case " + expr->toString() + ":\n" : "default:\n");
        ret += stmt->toString(ind);
        return ret;
    }
};

/**
 * The node class holding a switch statement in the parse tree.
 */
struct SwitchNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* body;

    //
    // NOTE: the following variables will be computed after calling analyze function
    //
    bool hasDefaultLabel = false;                   // Whether this switch has default label or not
    unordered_map<int, CaseLabelNode*> caseMap;     // Map of all case constant integer values
    ExprList caseLabels;                            // List of all case expression nodes in this switch
    vector<StmtList> caseStmts;                     // List of statements corresponding to each case label
    VarList initializedVars;                        // List of declared variables with initialization, used to detect cross variables initialization


    SwitchNode(const Location& loc, ExpressionNode* cond, StatementNode* body) : StatementNode(loc) {
        this->cond = cond;
        this->body = body;
    }

    virtual ~SwitchNode() {
        if (cond) delete cond;
        if (body) delete body;
    }

    virtual void populate() {
        BlockNode* block = dynamic_cast<BlockNode*>(body);

        if (block == NULL) {
            addCaseBlock(body);
            return;
        }

        for (int i = 0; i < block->statements.size(); ++i) {
            addCaseBlock(block->statements[i]);
        }
    }

    virtual void addCaseBlock(StatementNode* stmt) {
        CaseLabelNode* caseLabel;

        while (caseLabel = dynamic_cast<CaseLabelNode*>(stmt)) {
            caseLabels.push_back(caseLabel->expr);
            caseStmts.push_back(StmtList());

            stmt = caseLabel->stmt;
        }

        if (caseStmts.size() > 0) {
            caseStmts.back().push_back(stmt);
        }
    }

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "switch (" + cond->toString() + ")\n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));

        //
        // Switch case block population debugging code
        //

        // string ret = string(ind, ' ') + "switch (" + cond->toString() + ")\n";
        // ret += string(ind, ' ') + "{\n";
        //
        // for (int i = 0; i < caseLabels.size(); ++i) {
        //     if (caseLabels[i] != NULL) {
        //         ret += string(ind, ' ') + "case " + caseLabels[i]->toString() + ":\n";
        //     } else {
        //         ret += string(ind, ' ') + "default:\n";
        //     }
        //
        //     for (int j = 0; j < caseStmts[i].size(); ++j) {
        //         ret += caseStmts[i][j]->toString(ind + 4) + "\n";
        //     }
        // }
        //
        // ret += string(ind, ' ') + "}";

        return ret;
    }
};

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

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "while (" + cond->toString() + ") \n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
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

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "do\n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4)) + "\n";
        ret += string(ind, ' ') + "while (" + cond->toString() + ");";
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

    ForNode(const Location& loc, StatementNode* initStmt, ExpressionNode* cond, ExpressionNode* inc,
            StatementNode* body) : StatementNode(loc) {
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

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "for (";
        ret += (initStmt ? initStmt->toString() : "") + ";";
        ret += (cond ? cond->toString() : "") + ";";
        ret += (inc ? inc->toString() : "") + ")\n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
        return ret;
    }
};

/**
 * The node class holding a break statement in the parse tree.
 */
struct BreakStmtNode : public StatementNode {

    BreakStmtNode(const Location& loc) : StatementNode(loc) {}

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + "break";
    }
};

/**
 * The node class holding a continue statement in the parse tree.
 */
struct ContinueStmtNode : public StatementNode {

    ContinueStmtNode(const Location& loc) : StatementNode(loc) {}

    virtual bool analyze(ScopeContext* context);

    virtual string generateQuad(GenerationContext* context);

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + "continue";
    }
};

#endif