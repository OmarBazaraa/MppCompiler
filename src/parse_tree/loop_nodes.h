#ifndef __LOOP_NODES_H_
#define __LOOP_NODES_H_

#include "../context/quadruple_context.h"
#include "basic_nodes.h"
#include "statement_nodes.h"


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
        if (cond) {
            delete cond;
        }
        if (body) {
            delete body;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "while (";
        cond->print(0);
        cout << ") " << endl;
        body->print(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
    }
	
	virtual void generateQuad(QuadrupleContext* quadContext) {
		int label1 = quadContext->labelCounter++, label2 = quadContext->labelCounter++;
		cout << "L" << label1 << ":" << endl;
		cond->generateQuad(quadContext);
		cout << "JZ L" << label2 << endl;
		
		quadContext->breakLabels.push(label2);
		quadContext->continueLabels.push(label1);
		
		body->generateQuad(quadContext);
		
		quadContext->breakLabels.pop();
		quadContext->continueLabels.pop();
		
		cout << "JMP L" << label1 << endl;
		cout << "L" << label2 << ":" << endl;
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
        if (cond) {
            delete cond;
        }
        if (body) {
            delete body;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "do" << endl;
        body->print(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
        cout << endl;
        cout << string(ind, ' ') << "while (";
        cond->print(0);
        cout << ");";
    }
	
	virtual void generateQuad(QuadrupleContext* quadContext) {
		int label1 = quadContext->labelCounter++, label2 = quadContext->labelCounter++, label3 = quadContext->labelCounter++;
		cout << "L" << label1 << ":" << endl;
		
		quadContext->breakLabels.push(label3);
		quadContext->continueLabels.push(label2);
		
		body->generateQuad(quadContext);
		
		quadContext->breakLabels.pop();
		quadContext->continueLabels.pop();
		
		cout << "L" << label2 << ":" << endl;
		cond->generateQuad(quadContext);
		cout << "JNZ L" << label1 << endl;
		cout << "L" << label3 << ":" << endl;
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
	virtual void generateQuad(QuadrupleContext* quadContext) {
		int label1 = quadContext->labelCounter++;
		int label2 = quadContext->labelCounter++;
		int label3 = quadContext->labelCounter++;
		int label4 = quadContext->labelCounter++;
		int label5 = quadContext->labelCounter++;			
																				
		initStmt->generateQuad(quadContext);									
		cout << "L" << label1 << ":" << endl;									
		cond->generateQuad(quadContext);										
		cout << "JMP L" << label4 << endl;										
		cout << "L" << label2 << ":" << endl;									
		inc->generateQuad(quadContext);											
		cout << "JMP L" << label1 << endl;										
		cout << "L" << label3 << ":" << endl;	
		
		quadContext->breakLabels.push(label5);
		quadContext->continueLabels.push(label2);
		
		body->generateQuad(quadContext);
		
		quadContext->breakLabels.pop();
		quadContext->continueLabels.pop();
		
		cout << "JMP L" << label2 << endl;										
		cout << "L" << label4 << ":" << endl;									
		cout << "JZ L" << label5 << endl;										
		cout << "JMP L" << label3 << endl;										
		cout << "L" << label5 << ":" << endl;
    }
};

#endif