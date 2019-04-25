#ifndef __PARSE_TREE_H_
#define __PARSE_TREE_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Node;
struct StatementNode;
struct VarDeclarationNode;
struct ExpressionNode;

typedef vector<VarDeclarationNode*> VarList;
typedef vector<ExpressionNode*> ExprList;


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
    int type;
    string name;
    ExpressionNode* value;
    bool isConst;

    VarDeclarationNode(int type, const char* name, ExpressionNode* value = NULL, bool isConst = false) {
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
        cout << string(ind, ' ') << (isConst ? "const " : "var ") << name;

        if (value) {
            cout << " = ";
            value->print(0);
        }
    }
};

struct FunctionNode : public StatementNode {
    int type;
    string name;
    VarList paramList;
    BlockNode* body;

    FunctionNode(int type, const char* name, const VarList& paramList, BlockNode* body) {
        this->type = type;
        this->name = string(name);
        this->paramList = paramList;
        this->body = body;
    }

    virtual ~FunctionNode() {
        for (int i = 0; i < paramList.size(); ++i) {
            delete paramList[i];
        }
        delete body;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "func " << name << "(";

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
        delete value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << name << " = ";
        value->print(0);
    }
};

struct BinaryOprNode : public ExpressionNode {
    int opr;
    ExpressionNode* lhs;
    ExpressionNode* rhs;

    BinaryOprNode(int opr, ExpressionNode* lhs, ExpressionNode* rhs) {
        this->opr = opr;
        this->lhs = lhs;
        this->rhs = rhs;
    }

    virtual ~BinaryOprNode() {
        delete lhs;
        delete rhs;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ');
        lhs->print(0);
        cout << ' ' << opr << ' ';
        rhs->print(0);
    }
};

struct UnaryOprNode : public ExpressionNode {
    int opr;
    ExpressionNode* expr;

    UnaryOprNode(int opr, ExpressionNode* expr) {
        this->opr = opr;
        this->expr = expr;
    }

    virtual ~UnaryOprNode() {
        delete expr;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ');
        cout << opr << ' ';
        expr->print(0);
    }
};

#endif