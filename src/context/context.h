#ifndef __CONTEXT_H_
#define __CONTEXT_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "../utils/utils.h"
#include "../utils/consts.h"

using namespace std;


/**
 * Struct holding the current context in the semantic analyzing phase.
 */
class Context {
    string sourceFilename;
    vector<string> sourceCode;

public:

    /**
     * Constructs a new context object.
     */
    Context(const string& sourceFilename) {
        this->sourceFilename = sourceFilename;
        this->readSourceCode();
    }

    /**
     * Prints error message at the given location in this context.
     */
    void printError(const string& what, const Location& loc) {
        fprintf(stderr, "%s:%d:%d: error: %s\n", sourceFilename.c_str(), loc.lineNum, loc.pos, what.c_str());
        fprintf(stderr, "%s\n", sourceCode[loc.lineNum - 1].c_str());
        fprintf(stderr, "%*s\n", loc.pos, "^");
    }

private:
    /**
     * Reads the given source code file and fills
     * the global vector {@code sourceCode}.
     */
    void readSourceCode() {
        // Open input file
        ifstream fin(sourceFilename);

        // Check that the file was opened successfully
        if (!fin.is_open()) {
            return;
        }

        // Read the source code line by line
        string line;
        while (getline(fin, line)) {
            sourceCode.push_back(line);
        }

        // Close
        fin.close();
    }
};

#endif