#ifndef __CONTEXT_H_
#define __CONTEXT_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "../symbol_table/symbol_table.h"

#include "../utils/utils.h"
#include "../utils/consts.h"

using namespace std;


/**
 * Struct holding scope information.
 */
struct Scope {
    ScopeType type;
    SymbolTable table;
    Symbol* sym;

    Scope(ScopeType type, Symbol* sym = NULL) {
        this->type = type;
        this->sym = sym;
    }
};

/**
 * Struct holding the current context in the semantic analyzing phase.
 */
class ScopeContext {
private:
    //
    // Private member variables
    //
    string sourceFilename;
    vector<string> sourceCode;
    vector<Scope*> scopes;

public:
    //
    // Public member variables
    //
    bool declareFuncParams = false;

public:

    /**
     * Constructs a new context object.
     */
    ScopeContext(const string& sourceFilename) {
        this->sourceFilename = sourceFilename;
        this->readSourceCode();
    }

    /**
     * Adds a new scope to this context.
     * 
     * @param type the type of the scope to add.
     */
    void addScope(ScopeType type, Symbol* sym = NULL) {
        scopes.push_back(new Scope(type, sym));
    }

    /**
     * Removes the lastly added scope from this context.
     */
    void popScope() {
        delete scopes.back();
        scopes.pop_back();
    }

    /**
     * Checks whether the current scope is the global scope within this context or not.
     * 
     * @return {@code true} if the current scope is the global scope; {@code false} otherwise.
     */
    bool isGlobalScope() {
        return scopes.size() == 1;
    }

    /**
     * Declares a new symbol in the the lastly added scope in this context.
     * 
     * @param sym the symbol to add.
     * 
     * @return {@code true} if the symbol was declared successfully; {@code false} if already declared.
     */
    bool declareSymbol(Symbol* sym) {
        SymbolTable& table = scopes.back()->table;

        if (table.count(sym->identifier)) {
            return false;
        }

        table[sym->identifier] = sym;
        return true;
    }

    /**
     * Searches for the given identifier in the symbol table.
     * 
     * @param identifier the name of the symbol to search for.
     * 
     * @return a pointer on the found symbol table entry, or {@code NULL} if not available.
     */
    Symbol* getSymbol(const string& identifier) {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->table.count(identifier)) {
                return scopes[i]->table[identifier];
            }
        }

        return NULL;
    }

    /**
     * Searches for the inner most function scope and returns its definition.
     * 
     * @return a pointer to the function definition if found, or {@code NULL} if not available.
     */
    Symbol* getFunctionSymbol() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_FUNCTION) {
                return scopes[i]->sym;
            }
        }

        return NULL;
    }

    /**
     * Checks whether this context has a scope that can accept
     * break statement or not. That is, a loop scope or switch scope.
     * 
     * @return {@code true} if this context has a break scope, {@code false} otherwise.
     */
    bool hasBreakScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_LOOP || scopes[i]->type == SCOPE_SWITCH) {
                return true;
            }
        }

        return false;
    }

    /**
     * Checks whether this context has a scope that can accept
     * continue statement or not. That is, a loop scope.
     * 
     * @return {@code true} if this context has a continue scope, {@code false} otherwise.
     */
    bool hasLoopScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_LOOP) {
                return true;
            }
        }

        return false;
    }

    /**
     * Checks whether this context has a scope that can accept
     * return statement or not. That is, a function scope.
     * 
     * @return {@code true} if this context has a continue scope, {@code false} otherwise.
     */
    bool hasFunctionScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_FUNCTION) {
                return true;
            }
        }

        return false;
    }

    /**
     * Prints error message at the given location in this context.
     */
    void printError(const string& what, const Location& loc) {
        fprintf(stderr, "%s:%d:%d: error: %s\n", sourceFilename.c_str(), loc.lineNum, loc.pos, what.c_str());
        fprintf(stderr, "%s\n", sourceCode[loc.lineNum - 1].c_str());
        fprintf(stderr, "%*s", loc.pos, "^");

        if (loc.len > 1) {
            fprintf(stderr, "%s", string(loc.len - 1, '~').c_str());
        }

        fprintf(stderr, "\n");
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