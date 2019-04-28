#ifndef __STATEMENT_NODES_H_
#define __STATEMENT_NODES_H_

#include "../context/generation_context.h"
#include "basic_nodes.h"
#include "value_nodes.h"


/**
 * The node class holding a block of code in the parse tree.
 */
struct BlockNode : public StatementNode {
    StmtList statements;

    BlockNode() {

    }

    BlockNode(const Location& loc) : StatementNode(loc) {

    }

    BlockNode(const Location& loc, const StmtList& statements) : StatementNode(loc) {
        this->statements = statements;
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
    
    virtual void generateQuad(GenerationContext* generationContext) {
        for (int i = 0; i < statements.size(); ++i) {
            statements[i]->generateQuad(generationContext);
        }
    }
};

/**
 * The node class holding a variable or constant declaration statement in the parse tree.
 */
struct VarDeclarationNode : public StatementNode {
    TypeNode* type;
    IdentifierNode* name;
    ExpressionNode* value;
    bool isConst;

    VarDeclarationNode(TypeNode* type, IdentifierNode* name, ExpressionNode* value = NULL, bool isConst = false) : StatementNode(type->loc) {
        this->type = type;
        this->name = name;
        this->value = value;
        this->isConst = isConst;
    }

    virtual ~VarDeclarationNode() {
        if (type) {
            delete type;
        }
        if (name) {
            delete name;
        }
        if (value) {
            delete value;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ');

        if (isConst) {
            cout << "const ";
        }

        type->print(0);
        cout << " ";
        name->print(0);

        if (value) {
            cout << " = ";
            value->print(0);
        }
    }
    
    virtual void generateQuad(GenerationContext* generationContext) {
        if (value) {
            value->generateQuad(generationContext);
        }
        
        cout << "POP " << name->name << endl; 
    }
};

/**
 * The node class holding a break statement in the parse tree.
 */
struct BreakStmtNode : public StatementNode {

    BreakStmtNode(const Location& loc) : StatementNode(loc) {

    }

    virtual ~BreakStmtNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "break;";
    }
    
    virtual void generateQuad(GenerationContext* generationContext) {
        cout << "JMP L" << generationContext->breakLabels.top() << endl; 
    }
};

/**
 * The node class holding a continue statement in the parse tree.
 */
struct ContinueStmtNode : public StatementNode {

    ContinueStmtNode(const Location& loc) : StatementNode(loc) {

    }

    virtual ~ContinueStmtNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "continue;";
    }
    
    virtual void generateQuad(GenerationContext* generationContext) {
        cout << "JMP L" << generationContext->continueLabels.top() << endl; 
    }
};

/**
 * The node class holding a return statement in the parse tree.
 */
struct ReturnStmtNode : public StatementNode {
    ExpressionNode* value;

    ReturnStmtNode(const Location& loc, ExpressionNode* value) : StatementNode(loc) {
        this->value = value;
    }

    virtual ~ReturnStmtNode() {
        if (value) {
            delete value;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "return";
        if (value) {
            cout << " ";
            value->print(0);
        }
        cout << ";";
    }
    
    virtual void generateQuad(GenerationContext* generationContext) {
        if (value)
            value->generateQuad(generationContext);
        
        cout << "RET" << endl;
    }
};

/**
 * The node class holding a case statement in the parse tree.
 */
struct CaseStmtNode : public StatementNode {
    ExpressionNode* expr;
    StmtList body;
    bool isDefault;

    CaseStmtNode(const Location& loc, ExpressionNode* expr, const StmtList& body, bool isDefault = false) : StatementNode(loc) {
        this->expr = expr;
        this->body = body;
        this->isDefault = isDefault;
    }

    virtual ~CaseStmtNode() {
        if (expr) {
            delete expr;
        }
        for (int i = 0; i < body.size(); ++i) {
            delete body[i];
        }
    }

    virtual void print(int ind = 0) {
        if (isDefault) {
            cout << string(ind, ' ') << "default:" << endl;
        } else {
            cout << string(ind, ' ') << "case ";
            expr->print(0);
            cout << ":" << endl;
        }
        for (int i = 0; i < body.size(); ++i) {
            body[i]->print(ind + 4);
            cout << endl;
        }
    }
    
    virtual void generateQuad(GenerationContext* generationContext) {
        if (isDefault) {
            for (int i = 0; i < body.size(); ++i) {
                body[i]->generateQuad(generationContext);
            }
        }
        else {
            int label1 = generationContext->labelCounter++;
            expr->generateQuad(generationContext);
            cout << "JZ L" << label1 << endl;
            
            for (int i = 0; i < body.size(); ++i) {
                body[i]->generateQuad(generationContext);
            }
            
            cout << "L" << label1 << endl;
        }
    }
};

#endif