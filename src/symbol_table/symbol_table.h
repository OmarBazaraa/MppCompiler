#ifndef __SYMBOL_TABLE_H_
#define __SYMBOL_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

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
        return Utils::dtypeToStr(type) + " " + identifier;;
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
};

/**
 * Function symbol table entry.
 */
struct Func : public Symbol {
    vector<Var> paramList;

    virtual string header() {
        string ret = Utils::dtypeToStr(type) + " " + identifier + "(";
        for (int i = 0; i < paramList.size(); ++i) {
            if (i > 0) {
                ret += ", ";
            }
            ret += Utils::dtypeToStr(paramList[i].type);
        }
        ret += ")";
        return ret;
    }
};

/**
 * Symbol table type.
 */
typedef unordered_map<string, Symbol*> SymbolTable;

#endif