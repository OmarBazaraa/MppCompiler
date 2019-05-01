#ifndef __SYMBOL_TABLE_H_
#define __SYMBOL_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../utils/utils.h"
#include "../utils/consts.h"

using namespace std;


/**
 * The base class for the symbol table entries.
 */
struct Symbol {
    DataType type;
    string identifier;

    virtual string header() {
        return Utils::dtypeToStr(type) + " " + identifier;
    }

    virtual string getType() {
        return Utils::dtypeToStr(type);
    }
};

/**
 * Variable symbol table entry.
 */
struct Var : public Symbol {
    bool isConst = false;
    bool isInitialized = false;

    virtual string header() {
        return (isConst ? "const " : "") + Utils::dtypeToStr(type) + " " + identifier;
    }

    virtual string getType() {
        return Utils::dtypeToStr(type);
    }
};

/**
 * Function symbol table entry.
 */
struct Func : public Symbol {
    vector<Var> paramList;

    virtual string header() {
        string ret = Utils::dtypeToStr(type) + " " + identifier + "(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + Utils::dtypeToStr(paramList[i].type);
        }
        ret += ")";
        return ret;
    }

    virtual string getType() {
        string ret = Utils::dtypeToStr(type) + "(*)(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + Utils::dtypeToStr(paramList[i].type);
        }
        ret += ")";
        return ret;
    }
};

/**
 * Switch statement scope information.
 */
struct Switch {
    bool defaultLabel = false;
    unordered_set<int> labels;
};

/**
 * Symbol table type.
 */
typedef unordered_map<string, Symbol*> SymbolTable;

/**
 * Struct holding scope information.
 */
struct Scope {
    ScopeType type;
    SymbolTable table;

    Scope(ScopeType type) {
        this->type = type;
    }
};

#endif