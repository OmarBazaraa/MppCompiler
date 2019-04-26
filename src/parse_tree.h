#ifndef __PARSE_TREE_H_
#define __PARSE_TREE_H_

#include <iostream>
#include <string>
#include <vector>

#include "consts.h"
#include "utils.h"

using namespace std;

//
// Prototypes
//

struct Node;
struct StatementNode;
struct VarDeclarationNode;
struct ExpressionNode;

typedef vector<VarDeclarationNode*> VarList;
typedef vector<ExpressionNode*> ExprList;

// ===========================================================================

struct Node {
    virtual ~Node() {}

    virtual void print(int ind = 0) {
        
    }
};

struct StatementNode : public Node {
    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << ";" ;
    }
};

struct ExpressionNode : public StatementNode {

};

// ===========================================================================

struct BlockNode : public StatementNode {
    vector<StatementNode*> statements;

    BlockNode() {

    }

    virtual ~BlockNode() {
        for (int i = 0; i < statements.size(); ++i) {
            delete statements[i];
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "{" << endl;
        for (int i = 0; i < statements.size(); ++i) {
            statements[i]->print(ind + 4);
            cout << endl;
        }
        cout << string(ind, ' ') << "}";
    }
};

struct VarDeclarationNode : public StatementNode {
    DataType type;
    string name;
    ExpressionNode* value;
    bool isConst;

    VarDeclarationNode(DataType type, const char* name, ExpressionNode* value = NULL, bool isConst = false) {
        this->type = type;
        this->name = string(name);
        this->value = value;
        this->isConst = isConst;
    }

    virtual ~VarDeclarationNode() {
        if (value) {
            delete value;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ');

        if (isConst) {
            cout << "const ";
        }

        cout << Utils::dtypeToStr(type) << ' ' << name;

        if (value) {
            cout << " = ";
            value->print(0);
        }
    }
};

struct FunctionNode : public StatementNode {
    DataType type;
    string name;
    VarList paramList;
    BlockNode* body;

    FunctionNode(DataType type, const char* name, const VarList& paramList, BlockNode* body) {
        this->type = type;
        this->name = string(name);
        this->paramList = paramList;
        this->body = body;
    }

    virtual ~FunctionNode() {
        for (int i = 0; i < paramList.size(); ++i) {
            delete paramList[i];
        }
        if (body) {
            delete body;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << Utils::dtypeToStr(type) << ' ' << name << "(";

        if (paramList.size()) {
            paramList[0]->print(0);
        }

        for (int i = 1; i < paramList.size(); ++i) {
            cout << ", ";
            paramList[0]->print(0);
        }

        cout << ")" << endl;
        body->print(ind);
    }
};

struct IfStmtNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* ifBody;
    StatementNode* elseBody;

    IfStmtNode(ExpressionNode* cond, StatementNode* ifBody = NULL, StatementNode* elseBody = NULL) {
        this->cond = cond;
        this->ifBody = ifBody;
        this->elseBody = elseBody;
    }

    virtual ~IfStmtNode() {
        if (cond) {
            delete cond;
        }
        if (ifBody) {
            delete ifBody;
        }
        if (elseBody) {
            delete elseBody;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "if (";
        cond->print(0);
        cout << ")" << endl;
        ifBody->print(ind + (dynamic_cast<BlockNode*>(ifBody) ? 0 : 4));

        if (elseBody) {
            cout << endl;
            cout << string(ind, ' ') << "else" << endl;
            elseBody->print(ind + (dynamic_cast<BlockNode*>(elseBody) ? 0 : 4));
        }
    }
};

struct ForStmtNode : public StatementNode {
    StatementNode* initStmt;
    ExpressionNode* cond;
    ExpressionNode* inc;
    StatementNode* body;

    ForStmtNode(StatementNode* initStmt, ExpressionNode* cond, ExpressionNode* inc, StatementNode* body) {
        this->initStmt = initStmt;
        this->cond = cond;
        this->inc = inc;
        this->body = body;
    }

    virtual ~ForStmtNode() {
        if (initStmt) {
            delete initStmt;
        }
        if (cond) {
            delete cond;
        }
        if (inc) {
            delete inc;
        }
        if (body) {
            delete body;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "for (";

        if (initStmt) {
            initStmt->print(0);
        }
        cout << "; ";

        if (cond) {
            cond->print(0);
        }
        cout << "; ";

        if (inc) {
            inc->print(0);
        }

        cout << ")" << endl;

        body->print(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
    }
};

// ===========================================================================

struct IntNode : public ExpressionNode {
    int value;

    IntNode(int value) {
        this->value = value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

struct FloatNode : public ExpressionNode {
    float value;

    FloatNode(float value) {
        this->value = value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

struct CharNode : public ExpressionNode {
    char value;

    CharNode(char value) {
        this->value = value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

struct BoolNode : public ExpressionNode {
    bool value;

    BoolNode(bool value) {
        this->value = value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

struct VarNode : public ExpressionNode {
    string name;

    VarNode(const char* name) {
        this->name = string(name);
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << name;
    }
};

struct FunctionCallNode : public ExpressionNode {
    string name;
    ExprList argList;

    FunctionCallNode(const char* name, const ExprList& argList) {
        this->name = string(name);
        this->argList = argList;
    }

    virtual ~FunctionCallNode() {
        for (int i = 0; i < argList.size(); ++i) {
            delete argList[i];
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << name << "(";

        if (argList.size()) {
            argList[0]->print(0);
        }

        for (int i = 1; i < argList.size(); ++i) {
            cout << ", ";
            argList[0]->print(0);
        }

        cout << ")";
    }
};

struct AssignOprNode : public ExpressionNode {
    string name;
    ExpressionNode* value;

    AssignOprNode(const char* name, ExpressionNode* value) {
        this->name = string(name);
        this->value = value;
    }

    virtual ~AssignOprNode() {
        if (value) {
            delete value;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "(" << name << " = ";
        value->print(0);
        cout << ")";
    }
};

struct BinaryOprNode : public ExpressionNode {
    Operator opr;
    ExpressionNode* lhs;
    ExpressionNode* rhs;

    BinaryOprNode(Operator opr, ExpressionNode* lhs, ExpressionNode* rhs) {
        this->opr = opr;
        this->lhs = lhs;
        this->rhs = rhs;
    }

    virtual ~BinaryOprNode() {
        if (lhs) {
            delete lhs;
        }
        if (rhs) {
            delete rhs;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "(";
        lhs->print(0);
        cout << ' ' << Utils::oprToStr(opr) << ' ';
        rhs->print(0);
        cout << ")";
    }
};

struct UnaryOprNode : public ExpressionNode {
    Operator opr;
    ExpressionNode* expr;

    UnaryOprNode(Operator opr, ExpressionNode* expr) {
        this->opr = opr;
        this->expr = expr;
    }

    virtual ~UnaryOprNode() {
        if (expr) {
            delete expr;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "(";

        if (opr == OPR_SUF_INC || opr == OPR_SUF_DEC) {
            expr->print(0);
            cout << Utils::oprToStr(opr);
        } else {
            cout << Utils::oprToStr(opr);
            expr->print(0);
        }

        cout << ")";
    }
};

#endif