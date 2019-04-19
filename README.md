# Compiler
A compiler for a simple programming language using Lex and Yacc.

# How to use
### 1. Install Lex & Yacc
Download and install **Lex** & **Yacc** compiler generating packages.

This compiler is built on:

| Package                 | Version        |
| ----------------------- | -------------- |
| Lex                     | Flex v2.6.4    |
| Yacc                    | Bison v3.3.1   |

**Download links:** [Windows](https://github.com/lexxmark/winflexbison/releases), Linux, Mac

# Lex usage
### Running Lex
To generate the lexer code using **_Flex_**, type the following command:

```Console
flex -o <output>.c <input>.l
```

Where `<input>.l` is the input lex specifications filename, and `<output>.c` is the name of the generated lexer C file.

To compiler the generated lexer program, type the following command:

```Console
gcc <output>.c -o <lexer_name>.exe
```

Where `<output>.c` is the name of the generated lexer C file, and `<lexer_name>.exe` is the name of the compiled lexer program.

### Structure of a Lex Specifcation
Lex specification file consists of three parts: the **definition** section, the **rules** section, and the **user subroutines** section.

```
... definition section ...
%%
... rules section ...
%%
... user subroutines section ...
```

#### 1. Definition Section
The definition section can include the literal block, definitions, internal table declarations, start conditions, and translations.

#### 2. Rules Section
The rules section contains pattern lines and C code.
A line that starts with whitespace, or material enclosed in `%{` and `%}` is C code.
A line that starts with anything else is a pattern line. 

Pattern lines contain a pattern (regular expression) followed by some whitespace and an action (C code) to execute when the input matches the pattern. If the C code is more than one statement or spans multiple lines, it must be enclosed in braces.

#### 3. User Subroutines Section
The contents of the user subroutines section is copied literally by lex to the C file.

This section typically includes routines called from the rules. If you redefine `input()`, `unput()`, `output()`, or `yywrap()`, the new versions or supporting subroutines might be here.

### Literal Block
The literal block in the definition section is C code bracketed by the lines `%{` and `%}`.

```
%{
... C code and declarations ...
%}
```

The contents of the literal block are copied literally to the generated C source file near the beginning, before the beginning of `yylex()`. The literal block usually contains declarations of variables and functions used by code in the rules section, as well as `#include` lines for header files. 
