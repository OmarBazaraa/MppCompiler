#ifndef __UTILS_H_
#define __UTILS_H_

#include <string>

#include "consts.h"

using namespace std;


/**
 * Struct holding the location information of the tokens.
 */
struct Location {
    int lineNum;
    int pos;
    int len;
};

/**
 * Struct holding the basic information of the tokens.
 */
struct Token {
    char* value;
    Location loc;
};

/**
 * Collection of utility functions to be used across the entire project modules.
 *
 * Note that all methods in this class must be static methods.
 */
struct Utils {

    /**
     * Converts the given operator into its corresponding token string.
     * 
     * @param opr the operator to convert.
     * 
     * @return the corresponding token string.
     */
    static string oprToStr(Operator opr) {
        switch (opr) {
            case OPR_ASSIGN:
                return "=";
            case OPR_ADD:
            case OPR_U_PLUS:
                return "+";
            case OPR_SUB:
            case OPR_U_MINUS:
                return "-";
            case OPR_MUL:
                return "*";
            case OPR_DIV:
                return "/";
            case OPR_MOD:
                return "%";
            case OPR_PRE_INC:
            case OPR_SUF_INC:
                return "++";
            case OPR_PRE_DEC:
            case OPR_SUF_DEC:
                return "--";
            case OPR_AND:
                return "&";
            case OPR_OR:
                return "|";
            case OPR_XOR:
                return "^";
            case OPR_NOT:
                return "~";
            case OPR_SHL:
                return "<<";
            case OPR_SHR:
                return ">>";
            case OPR_LOGICAL_AND:
                return "&&";
            case OPR_LOGICAL_OR:
                return "||";
            case OPR_LOGICAL_NOT:
                return "!";
            case OPR_GREATER:
                return ">";
            case OPR_GREATER_EQUAL:
                return ">=";
            case OPR_LESS:
                return "<";
            case OPR_LESS_EQUAL:
                return "<=";
            case OPR_EQUAL:
                return "==";
            case OPR_NOT_EQUAL:
                return "!=";
        }

        return "#";
    }
	
	/**
     * Converts the given operator into its corresponding quadruple string.
     * 
     * @param opr the operator to convert.
     * 
     * @return the corresponding quadruple string.
     */
	static string binOprToQuad(Operator opr) {
		switch (opr) {
            case OPR_ADD:
                return "ADD";
            case OPR_SUB:
                return "SUB";
            case OPR_MUL:
                return "MUL";
            case OPR_DIV:
                return "DIV";
            case OPR_MOD:
                return "MOD";
            case OPR_AND:
                return "AND";
            case OPR_OR:
                return "OR";
            case OPR_XOR:
                return "XOR";
            case OPR_NOT:
                return "NOT";
            case OPR_SHL:
                return "SHL";
            case OPR_SHR:
                return "SHR";
            case OPR_LOGICAL_AND:
                return "LAND";
            case OPR_LOGICAL_OR:
                return "LOR";
            case OPR_LOGICAL_NOT:
                return "LNOT";
            case OPR_GREATER:
                return "GT";
            case OPR_GREATER_EQUAL:
                return "GTE";
            case OPR_LESS:
                return "LT";
            case OPR_LESS_EQUAL:
                return "LTE";
            case OPR_EQUAL:
                return "EQU";
            case OPR_NOT_EQUAL:
                return "NEQ";
        }
		
		return "#";
	}

    /**
     * Converts the given operator into its corresponding token string.
     * 
     * @param opr the operator to convert.
     * 
     * @return the corresponding token string.
     */
    static string dtypeToStr(DataType type) {
        switch (type) {
            case DTYPE_INT:
                return "int";
            case DTYPE_FLOAT:
                return "float";
            case DTYPE_CHAR:
                return "char";
            case DTYPE_BOOL:
                return "bool";
            case DTYPE_VOID:
                return "void";
        }

        return "unknown";
    }
};

#endif