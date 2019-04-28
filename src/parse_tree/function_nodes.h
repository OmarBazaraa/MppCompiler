#ifndef __FUNCTION_NODES_H_
#define __FUNCTION_NODES_H_

#include "../context/quadruple_context.h"
#include "basic_nodes.h"
#include "statement_nodes.h"
#include "value_nodes.h"


/**
 * The node class holding a function in the parse tree.
 */
struct FunctionNode : public StatementNode {
    TypeNode* type;
    IdentifierNode* name;
    VarList paramList;
    BlockNode* body;
    Func func;

    FunctionNode(TypeNode* type, IdentifierNode* name, const VarList& paramList, BlockNode* body) : StatementNode(type->loc) {
        this->type = type;
        this->name = name;
        this->paramList = paramList;
        this->body = body;

        this->func.type = type->type;
        this->func.identifier = name->name;

        for (int i = 0; i < paramList.size(); ++i) {
            this->func.paramList.push_back(paramList[i]->var);
        }
    }

    virtual ~FunctionNode() {
        if (type) delete type;
        if (name) delete name;
        if (body) delete body;

        for (int i = 0; i < paramList.size(); ++i) {
            delete paramList[i];
        }
    }

    virtual bool analyze(ScopeContext* context) {
        if (!context->isGlobalScope()) {
            context->printError("a function-definition is not allowed here", name->loc);
            return false;
        }

        bool ret = true;

        if (!context->declareSymbol(&func)) {
            context->printError("'" + func.header() + "' redeclared", name->loc);
            ret = false;
        }

        context->addScope(SCOPE_FUNCTION);
        context->functions.push(&func);

        context->declareFuncParams = true;
        for (int i = 0; i < paramList.size(); ++i) {
            ret &= paramList[i]->analyze(context);
        }
        context->declareFuncParams = false;
        
        ret &= body->analyze(context);

        context->functions.pop();
        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + type->toString() + " " + name->toString() + "(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + paramList[i]->toString();
        }
        ret += ")\n";
        ret += body->toString(ind);
        return ret;
    }
	
	virtual void generateQuad(QuadrupleContext* quadContext) {
		cout << "PROC " << name->name << endl;
		for (int i = 0; i < paramList.size(); ++i) {
            paramList[i]->generateQuad(quadContext);
        }
		body->generateQuad(quadContext);
		cout << "ENDP " << name->name << endl;
    } 
};

/**
 * The node class holding a function call expression in the parse tree.
 */
struct FunctionCallNode : public ExpressionNode {
    IdentifierNode* name;
    ExprList argList;

    FunctionCallNode(IdentifierNode* name, const ExprList& argList) : ExpressionNode(name->loc) {
        this->name = name;
        this->argList = argList;
    }

    virtual ~FunctionCallNode() {
        if (name) delete name;

        for (int i = 0; i < argList.size(); ++i) {
            delete argList[i];
        }
    }

    virtual bool analyze(ScopeContext* context) {
        bool ret = true;

        Symbol* ptr = context->getSymbol(name->name);
        Func* func = dynamic_cast<Func*>(ptr);

        if (ptr == NULL) {
            context->printError("'" + name->name + "' was not declared in this scope", loc);
            ret = false;
        }
        else if (func == NULL) {
            context->printError("'" + name->name + "' cannot be used as a function", loc);
            ret = false;
        }
        else if (argList.size() > func->paramList.size()) {
            context->printError("too many arguments to function '" + func->header() + "'", loc);
            ret = false;
        }
        else if (argList.size() < func->paramList.size()) {
            context->printError("too few arguments to function '" + func->header() + "'", loc);
            ret = false;
        } else {
            type = ptr->type;
        }

        for (int i = 0; i < argList.size(); ++i) {
            if (!argList[i]->analyze(context)) {
                ret = false;
                continue;
            }

            if (func && argList[i]->type == DTYPE_VOID || argList[i]->type == DTYPE_FUNC_PTR) {
                context->printError("invalid conversion from '" + argList[i]->getType() + "' to '" +
                    func->paramList[i].getType() + "' in function '" + 
                    func->header() + "' call", argList[i]->loc);
                return false;
            }
        }

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + name->toString() + "(";
        for (int i = 0; i < argList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + argList[i]->toString();
        }
        return ret += ")";
    }
	
	virtual void generateQuad(QuadrupleContext* quadContext) {
		for (int i = argList.size() - 1; i >= 0; --i) {
            argList[i]->generateQuad(quadContext);
        }
		cout << "CALL " << name->name << endl;
    }
};

#endif