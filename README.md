# M++ Compiler
M++ is a simple programming language compiler similar to C++ built using Lex and Yacc.

For more information about M++, please refer to this [link](https://github.com/OmarBazaraa/MppCompiler/blob/master/docs/M%2B%2B%20Description.pdf).

# M++ IDE
M++ comes with a very simple IDE that enables us view, edit, and compile `mpp` files, and display
the resulting quadruples and symbol table.

![alt text](https://raw.githubusercontent.com/OmarBazaraa/MppCompiler/master/img/ide.png)

For more information on how to build and use, please refer to the [gui branch](https://github.com/OmarBazaraa/MppCompiler/tree/gui). 

# How to Use
### 1. Install Lex & Yacc  
Download and install **Lex** & **Yacc** compiler generating packages.

This compiler is built on:

| Package                 | Version        |
| ----------------------- | -------------- |
| Lex                     | Flex v2.6.4    |
| Yacc                    | Bison v3.3.1   |

**Download links:** [Windows](https://github.com/lexxmark/winflexbison/releases), Linux, Mac

### 2. Install GCC compiler

### 3. Build M++ compiler
Run `make build` from the repository's main folder to build the compiler.

### 4. Run M++ compiler
Run `make run` to run the recently built compiler.  

**_Note:_** You can change the input file from the `Makefile`.

# M++ Compiler Commands
**Syntax**:  
`M++ [-h|--help] [-v|--version] [-w|--warn] [-o|--output <output_file>] [-s|--sym_table <filename>]  <input_file>`

| Command                                         | Description                                                      |
| ----------------------------------------------- | ---------------------------------------------------------------- |
| `-h` or `--help`                                | Print help menu and exit.                                        |
| `-o` or `--output` `<filename>`                 | Specify the output filename.                                     |
| `-s` or `--sym_table` `<filename>`              | Output the symbol table to the given file                        |
| `-v` or `--version`                             | Print the installed version number and exit.                     |
| `-w` or `--warn`                                | Show warning messages.                                           |

# Overview
In this section, we are going to give a brief descriptions and examples for the syntax and semantics allowed by M++. As we said, it is almost identical to C-language but with less features.

## Data Types
In M++, we support the basic data types but unfortunately, we do not support arrays or pointers.
The supported types:
-	`void`: is only valid as a function return type to tell that it has no value to return.
-	`int`: is an integer numeric value data type.
-	`float`: is a real numeric value data type.
-	`char`: is a character value data type.
-	`bool`: is a Boolean value data type that accepts either `true` or `false`.

## Variable/Constant Declarations
In M++, we support scoped variables and constants declaration. Each variable or constant has its own scope, and multiple variable/constants can be declared with the same identifier only if they are in different scopes. As in C-language, constants must be initialized while being declared.

**e.g.**

```C++
const float PI = 3.14;
const float EPS = 1e-9;
int x;
char c = 'c';
bool flag = true;
int a = 0, b, MAX = 100;
```

## If-Else Control Statements
We support if-else control statement in almost the exact same way as in C-language. If the if-condition evaluates to a non-zero value, then the if-body will be executed. Otherwise, the else-body will be executed if exists. If-body and else-body can either be one statement, or can be multiple statements enclosed by a block.

**e.g.**

```C++
if (x) {
    if (y > 0)
        /* if-body */
    else if (z & 1)
        /* else-if-body */
    else
        /* else-body */
}
```

## Switch Statements
Like if-statement, we support switch-statement in almost the exact same way as in C-language. The switch-expression must be of integer value, and the case-expression must be a constant integer value. Also, multiple case-expressions that evaluate to the same value is not allowed. Like C, the code of the matched case will be executed and the execution will continue to the below code of other cases until a break-statement is found.


**e.g.**

```C++
switch (state) {
case 1:
case 2:
    /* do something */
case RUNNING: // RUNNING must be defined as constant
    /* do something */
    break;
default:
    /* default */
}
```

## For/While/Do-While Loops
M++ supports loops in almost the exact same way as in C-language. We support for-loops, while-loops, and do-while loops. Break-statements and continue-statements are supported within the scope of a loop, and they function like in C-language, they break or continue the execution of the inner most loop.

**e.g.**
```C++
for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
        while (i < j) // do something
        continue;
    }
}

do {
    if (cond)
        break;
    // do something
} while (true);
```

## Functions
M++ supports functions but with limited functionalities than that of the C-language. We do not support default parameters. We do not support neither function prototyping nor function overloading.
Return-statements are allowed within the scope of a function. And functions can only be defined in the global scope.

**e.g.**
```C++
int fibonacci(int n) {
    return fibonacci(n - 1) + fibonacci(n - 2);
}
```

## Expressions
In M++, we support complex expressions similar to those of C-language. We support almost the entire set of operators supported by C-language with the same precedence and associativity.

**e.g.**
```C++
(((++x) = y++) = (8 * 7 - MAX) ^ (1 << i)) = (z = 3);
```

## Comments
M++ supports the same comment styles as in C-language. The comments can either be:
-	Line comment
   ```C++
   // This is a line comment
   ```
-	Block comment (multi-line comment)
  ```C++
  /**
   * This is a block comment
   * that can span
   * multiple lines
   */
  ```

# Errors Detected by M++
## Syntax Errors
### Scope-related errors
1. Code blocks or statements (other than variable, constants, and function declaration/definition) in the global scope.
2. `continue`-statement outside `for`, `while`, or `do-while` scopes.
3. `break`-statement outside `for`, `while`, `do-while`, or `switch` scopes.
4. `return`-statement outside function scope.
5. `case` and `default` labels outside switch scope.

### Other errors
1. Variable or constant declared with type `void`.
2. Constant declaration without initialization.

## Semantic Errors
### Identifier/Expression-related errors
1. Identifier re-declaration in the same scope.
2. Undeclared identifier access.
3. Constant assignment after declaration.
4. Invalid operand types. (i.e. operands of type `void` or pointer to function).
5. Branch condition not Boolean convertible.
6. Float operand to modulus operator.
7. Float operand to bitwise operators.
8. Use of uninitialized variable.
9. Increment and decrement operators with `rvalue` operand.

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
