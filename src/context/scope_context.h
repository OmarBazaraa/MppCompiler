#ifndef __CONTEXT_H_
#define __CONTEXT_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

#include "../parse_tree/parse_tree.h"

#include "../utils/utils.h"
#include "../utils/consts.h"

using namespace std;


/**
 * Symbol table type.
 */
typedef unordered_map<string, DeclarationNode*> SymbolTable;

/**
 * Struct holding scope information.
 */
struct Scope {
    ScopeType type;         // The type of the scope
    Node* ptr;              // A pointer to the node of the scope
    SymbolTable table;      // The symbol table of this scope

    Scope(ScopeType type, Node* ptr = NULL) {
        this->type = type;
        this->ptr = ptr;
    }
};

/**
 * Class holding the current context in the semantic analyzing phase.
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
    vector<pair<int, DeclarationNode*>> symbols;    // Used just for printing the symbol table. NOT IMPORTANT!
    bool warn;

public:
    //
    // Public member variables
    //
    bool declareFuncParams = false;
    bool initializeVar = false;

public:

    /**
     * Constructs a new context object.
     *
     * @param sourceFilename the filename of the source code to compile.
     * @param warn           whether to show warning messages or not.
     */
    ScopeContext(const string& sourceFilename, bool warn = false) {
        this->sourceFilename = sourceFilename;
        this->readSourceCode();
        this->warn = warn;
    }

    /**
     * Adds a new scope to this context.
     *
     * @param type the type of the scope to add.
     * @param ptr  a pointer to the node of the scope in the parse tree.
     */
    void addScope(ScopeType type, Node* ptr = NULL) {
        scopes.push_back(new Scope(type, ptr));
    }

    /**
     * Removes the lastly added scope from this context.
     */
    void popScope() {
        Scope* scope = scopes.back();
        scopes.pop_back();

        for (auto& it : scope->table) {
            DeclarationNode* sym = it.second;

            if (sym->used <= 0) {
                if (dynamic_cast<VarDeclarationNode*>(sym)) {
                    log("the value of variable '" + sym->declaredHeader() + "' is never used", sym->ident->loc, LOG_WARNING);
                }
                else if (sym->ident->name != "main") {
                    log("function '" + sym->declaredHeader() + "' is never called", sym->ident->loc, LOG_WARNING);
                }
            }
            
            aliases[sym->ident->name]--;
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
     * Searches for the inner most function scope.
     *
     * @return a pointer on the found function scope, or {@code NULL} if not available.
     */
    FunctionNode* getFunctionScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_FUNCTION) {
                return (FunctionNode*) scopes[i]->ptr;
            }
        }

        return NULL;
    }

    /**
     * Searches for the inner most switch scope.
     *
     * @return a pointer on the found switch scope, or {@code NULL} if not available.
     */
    SwitchNode* getSwitchScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_SWITCH) {
                return (SwitchNode*) scopes[i]->ptr;
            }
        }

        return NULL;
    }

    /**
     * Declares a new symbol in the the lastly added scope in this context.
     *
     * @param sym the symbol to add in the symbol table.
     *
     * @return {@code true} if the symbol was declared successfully; {@code false} if already declared.
     */
    bool declareSymbol(DeclarationNode* sym) {
        SymbolTable& table = scopes.back()->table;

        if (table.count(sym->ident->name)) {
            return false;
        }

        // Add symbol for later printing
        symbols.push_back({ scopes.size() - 1, sym });

        // Form a new alias name for the identifier
        int num = aliases[sym->ident->name]++;

        if (num > 0) {
            sym->alias = sym->ident->name + "@" + to_string(num);
        } else {
            sym->alias = sym->ident->name;
        }

        table[sym->ident->name] = sym;
        return true;
    }

    /**
     * Searches for the given identifier in the symbol table.
     *
     * @param identifier the name of the symbol to search for.
     *
     * @return a pointer on the found symbol table entry, or {@code NULL} if not available.
     */
    DeclarationNode* getSymbol(const string& identifier) {
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
     * Logs the given message at the given location in this context.
     *
     * @param what  the message to log.
     * @param loc   the location of the token to point upon in this context.
     * @param level the log level of this message.
     */
    void log(const string& what, const Location& loc, LogLevel level) {
        string logLvl;

        switch (level) {
            // In future we may change the output stream of each level
            // and change th colors of the log level and the intended token
            case LOG_ERROR:
                logLvl = "error";
                break;
            case LOG_WARNING:
                if (!warn) {
                    // Suppress  warnings
                    return;
                }
                logLvl = "warning";
                break;
            case LOG_NOTE:
                logLvl = "note";
                break;
        }

        fprintf(stdout, "%s:%d:%d: %s: %s\n", sourceFilename.c_str(), loc.lineNum, loc.pos, logLvl.c_str(), what.c_str());
        fprintf(stdout, "%s\n", sourceCode[loc.lineNum - 1].c_str());
        fprintf(stdout, "%*s", loc.pos, "^");

        if (loc.len > 1) {
            fprintf(stdout, "%s", string(loc.len - 1, '~').c_str());
        }

        fprintf(stdout, "\n");
    }

    /**
     * Returns the symbol table as a string for visualization.
     *
     * @return a string representing the symbol table.
     */
    string getSymbolTableStr() {
        stringstream ss;

        ss << "+-------+---------------------------------------------------+---------------------+---------------------+-------+\n";
        ss << "| scope | type                                              | identifier          | alias               | used  |\n";
        ss << "+-------+---------------------------------------------------+---------------------+---------------------+-------+\n";

        for (int i = 0; i < symbols.size(); ++i) {
            int scope = symbols[i].first;
            DeclarationNode* sym = symbols[i].second;

            ss << "| " << left << setw(6) << scope;
            ss << "| " << left << setw(50) << sym->declaredType();
            ss << "| " << left << setw(20) << sym->ident->name;
            ss << "| " << left << setw(20) << sym->alias;
            ss << "| " << left << setw(6) << sym->used << "|\n";
            ss << "+-------+---------------------------------------------------+---------------------+---------------------+-------+\n";
        }

        return ss.str();
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