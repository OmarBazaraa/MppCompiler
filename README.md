# M++ Compiler
M++ is a simple programming language compiler similar to C++ built using Lex and Yacc.

For more information about M++, please refer to this [link](https://github.com/OmarBazaraa/MppCompiler/blob/master/docs/M%2B%2B%20Description.pdf).

# How to Use
### 1. Install Lex & Yacc  
Download and install **Lex** & **Yacc** compiler generating packages.

This compiler is built on:

| Package                 | Version        |
| ----------------------- | -------------- |
| Lex                     | Flex v2.6.4    |
| Yacc                    | Bison v3.3.1   |

**Download links:** [Windows](https://github.com/lexxmark/winflexbison/releases), Linux, Mac

### 2. Install GCC compiler.

### 3. Build M++ compiler.
Run `make build` from the repository's main folder to build the compiler.

### 4. Run M++ compiler.
Run `make run` to run the recently built compiler.  

**_Note:_** You can change the input file from the `Makefile`.

# M++ Compiler Commands
**Syntax**: `M++ [-h|--help] [-v|--version] [-o|--output <output_file>] [-s|--sym_table <filename>]  <input_file>`

| Command                                         | Description                                                      |
| ----------------------------------------------- | ---------------------------------------------------------------- |
| `-h` or `--help`                                | Print help menu and exit.                                        |
| `-v` or `--version`                             | Print the installed version number and exit.                     |
| `-o` or `--output` `<filename>`                 | Specify the output filename.                                     |
| `-s` or `--sym_table` `<filename>`              | Output the symbol table to the given file                        |

# Syntax Errors Detected by M++
### Scope-related errors
1. Code blocks or statements (other than variable, constants, and function declaration/definition) in the global scope.
2. `continue`-statement outside `for`, `while`, or `do-while` scopes.
3. `break`-statement outside `for`, `while`, `do-while`, or `switch` scopes.
4. `return`-statement outside function scope.
5. `case` and `default` labels outside switch scope.

### Other errors
1. Variable or constant declared with type `void`.
2. Constant declaration without initialization.

# Semantic Errors Detected by M++
### Identifier/Expression-related errors
1. Identifier re-declaration in the same scope.
2. Undeclared identifier access.
3. Constant assignment after declaration.
4. Invalid operand types. (i.e. operands of type `void` or pointer to function).
5. Float operand to modulus operator.
6. Float operand to bitwise operators.
7. Use of uninitialized variable.
8. Increment and decrement operators with `rvalue` operand.

### Switch-statement-related errors
1. `switch` and `case` statements with non integer expression.
2. `case`-statement with non-constant expression.
3. Multiple `default`-labels in `switch` scope.
4. Multiple `case`-labels with the same constant expression in `switch` scope.
5. Cross variables initialization in `switch`-statement.

### Function-related errors
1. Value returned in `void` function and vice-versa.
2. Void returned in value-typed function.
3. Variable or constant call as a function.
4. Function call with more/less arguments than its parameters.
5. Function call with invalid argument type (i.e. argument of type `void` or pointer to function).
6. Function parameter with default value.

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
For more information about lex, please refer to this [link](https://github.com/OmarBazaraa/Compiler/blob/master/README_LEX.md).

# How to Use GUI
The GUI is built using python2.7 and PyQt5.

To start the GUI:
1. Navigate to the `gui/` folder.
2. Run 
    ```Console 
    python2.7 main.py <MppCompiler> <Quadruplues>.txt <SymbolsTable>.txt
    ```