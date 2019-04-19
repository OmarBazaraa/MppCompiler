# Compiler
A compiler for a simple programming language using Lex and Yacc.

# How to Use
### 1. Install Lex & Yacc
Download and install **Lex** & **Yacc** compiler generating packages.

This compiler is built on:

| Package                 | Version        |
| ----------------------- | -------------- |
| Lex                     | Flex v2.6.4    |
| Yacc                    | Bison v3.3.1   |

**Download links:** [Windows](https://github.com/lexxmark/winflexbison/releases), Linux, Mac

# Lex Usage

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

### More About Lex
For more information about lex, please refer to this [link](https://github.com/OmarBazaraa/Compiler/blob/master/README_LEX.md)
