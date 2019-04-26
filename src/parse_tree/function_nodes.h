#ifndef __FUNCTION_NODES_H_
#define __FUNCTION_NODES_H_

#include "basic_nodes.h"
#include "statement_nodes.h"


/**
 * The node class holding a function in the parse tree.
 */
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

/**
 * The node class holding a function call expression in the parse tree.
 */
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


#endif