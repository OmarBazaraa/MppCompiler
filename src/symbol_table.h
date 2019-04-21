#ifndef __SYMBOL_TABLE_H_
#define __SYMBOL_TABLE_H_

#define SYM_MAX 100

struct symbol_table {
    char* name;
    double value;
} symTable[SYM_MAX];

struct symbol_table* symbol_look(char* s);

#endif