#ifndef __CONTEXT_H_
#define __CONTEXT_H_

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;


/**
 * Struct holding the current context in the quadruple generation phase.
 */
struct QuadrupleContext {

    stack<int> breakLabels, continueLabels;
	int labelCounter;

    QuadrupleContext() {
        labelCounter = 1;
    }
};

#endif