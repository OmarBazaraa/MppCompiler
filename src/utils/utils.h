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
     * Checks whether the given type is an integer data type.
     * 
     * @param type the type to check.
     * 
     * @return {@code true} if the given type is integer type; {@code false} otherwise.
     */
    static bool isIntegerType(DataType type) {
        return (type == DTYPE_BOOL || type == DTYPE_CHAR || type == DTYPE_INT);
    }

    /**
     * Converts the given operator into its corresponding token string.
     * 
     * @param type the type to convert.
     * 
     * @return the corresponding token string.
     */
    static string dtypeToStr(DataType type) {
        switch (type) {
            case DTYPE_VOID:
                return "void";
            case DTYPE_BOOL:
                return "bool";
            case DTYPE_CHAR:
                return "char";
            case DTYPE_INT:
                return "int";
            case DTYPE_FLOAT:
                return "float";
            case DTYPE_FUNC_PTR:
                return "pointer to function";
        }

        return "unknown";
    }
};

#endif