#ifndef __CONTEXT_H_
#define __CONTEXT_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


/**
 * Struct holding the current context in the semantic analyzing phase.
 */
struct Context {

    const vector<string>& sourceCode;

    Context(const vector<string>& sourceCode) : sourceCode(sourceCode) {
        
    }
};

#endif