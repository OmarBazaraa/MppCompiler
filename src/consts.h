#ifndef __CONSTS_H_
#define __CONSTS_H_

/**
 * Enum holding all the possible operators in out language.
 */
enum Operations {
    ASSIGN = 100,       // a = b
    PLUS,               // a + b
    UNARY_PLUS,         // +54
    MINUS,              // a - b
    UNARY_MINUS,        // -54
    MULTIPLY,           // a * b
    DIVIDE,             // a / b
    MOD,                // a % b
    PREFIX_INC,         // ++a
    SUFFIX_INC,         // a++
    PREFIX_DEC,         // --a
    SUFFIX_DEC,         // a--
    AND,                // a & b
    OR,                 // a | b
    XOR,                // a ^ b
    NOT,                // ~a
    SHIFT_LEFT,         // a << 1
    SHIFT_RIGHT,        // a >> 1
    LOGICAL_AND,        // a && b
    LOGICAL_OR,         // a || b
    LOGICAL_NOT,        // !a
    GREATER,            // a > b
    GREATER_EQUAL,      // a >= b
    LESS,               // a < b
    LESS_EQUAL,         // a <= b
    EQUAL,              // a == b
    NOT_EQUAL,          // a != b
};

#endif