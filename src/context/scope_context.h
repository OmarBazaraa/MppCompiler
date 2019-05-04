#ifndef __CONTEXT_H_
#define __CONTEXT_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>

#include "../symbol_table/symbol_table.h"

#include "../utils/utils.h"
#include "../utils/consts.h"

using namespace std;


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

    unordered_map<string, int> aliases;

public:
    //
    // Public member variables
    //
    bool declareFuncParams = false;
    bool initializeVar = false;
    stack<Func*> functions;
    stack<Switch*> switches;

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
    void addScope(ScopeType type) {
        scopes.push_back(new Scope(type));
    }

    /**
     * Removes the lastly added scope from this context.
     */
    void popScope() {
        Scope* scope = scopes.back();
        scopes.pop_back();

        for (auto& it : scope->table) {
            Symbol* sym = it.second;

            if (!sym->used) {
                if (dynamic_cast<Var*>(sym)) {
                    printWarning("the value of variable '" + sym->header() + "' is never used");
                }
                else if (sym->identifier != "main") {
                    printWarning("function '" + sym->header() + "' is never called");
                }
            }
            
            aliases[sym->identifier]--;
        }

        delete scope;
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

        // Form a new alias name for the identifier
        int num = aliases[sym->identifier]++;

        if (num > 0) {
            sym->alias = sym->identifier + "@" + to_string(num);
        } else {
            sym->alias = sym->identifier;
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
     * case statement or not. That is, a switch scope.
     *
     * @return {@code true} if this context has a switch scope, {@code false} otherwise.
     */
    bool hasSwitchScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_SWITCH) {
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
     * Prints warning message.
     */
    void printWarning(const string& what) {
        fprintf(stdout, "warning: %s\n", what.c_str());
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
            sourceCode.push_back(Utils::replaceTabsWithSpaces(line));
        }

        // Close
        fin.close();
    }
};

#endif