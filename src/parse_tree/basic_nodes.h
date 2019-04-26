#ifndef __BASIC_NODES_H_
#define __BASIC_NODES_H_

#include <iostream>
#include <string>
#include <vector>

#include "../utils/consts.h"
#include "../utils/utils.h"


//
// Prototypes
//

struct Node;
struct StatementNode;
struct VarDeclarationNode;
struct ExpressionNode;

typedef vector<VarDeclarationNode*> VarList;
typedef vector<ExpressionNode*> ExprList;


/**
 * The base class of all parse tree nodes.
 */
struct Node {
    virtual ~Node() {}

    virtual void print(int ind = 0) {
        
    }
};

/**
 * The base class statement nodes in the parse tree.
 */
struct StatementNode : public Node {
    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << ";" ;
    }
};

/**
 * The base class expression nodes in the parse tree.
 */
struct ExpressionNode : public StatementNode {

};

#endif