#include <iostream>
#include <string>
#include <string.h>

#include "context/scope_context.h"
#include "parse_tree/parse_tree.h"
#include "utils/utils.h"
#include "utils/consts.h"

using namespace std;

//
// Compiler definitions
//
#define LANG_NAME           "M++"
#define VERSION             "0.1"
#define VERSION_DATE        "Apr 26, 2019"

//
// External functions & variables
//
extern int yyparse();
extern FILE* yyin;
extern StatementNode* programRoot;

//
// Global Variables
//
string inputFilename;
string outputFilename = "out.o";
vector<string> sourceCode;

//
// Functions prototypes
//
void printHelp();
void printVersion();
void parseArguments(int argc, char* argv[]);


/**
 * Main driver program.
 * 
 * @param argc the number of arguments sent to the program.
 * @param argv the arguments them self as sent to the program.
 */
int main(int argc, char* argv[]) {
    // Parse incoming arguments
    parseArguments(argc, argv);

    // Construct a context file
    ScopeContext context(inputFilename);

    // Open input file for Lex & Yacc
    yyin = fopen(inputFilename.c_str(), "r");

    if (yyin == NULL) {
        fprintf(stderr, "Error: Could not open the input file '%s'!\n", inputFilename.c_str());
        exit(0);
    }

    // Construct the parse tree
    yyparse();

    // Apply semantic check and quadruple generation
    if (programRoot->analyze(&context)) {
        programRoot->print();
    }

    // Finalize and release allocated memory
    fclose(yyin);
    delete programRoot;

    return 0;
}

/**
 * Prints the help menu of the compiler into the
 * standard output stream, then terminates the program.
 */
void printHelp() {
    printf("%s version %s, %s\n\n", LANG_NAME, VERSION, VERSION_DATE);
    printf("Usage: %s [switches] <input_file>\n", LANG_NAME);
    printf("    -h, --help                   Print the help menu and exit.\n");
    printf("    -v, --version                Print the installed version number and exit.\n");
    printf("    -o, --output <filename>      Specify the output filename.\n");
    exit(0);
}

/**
 * Prints the version of the compiler into the
 * standard output stream, then terminates the program.
 */
void printVersion() {
    printf("%s version %s, %s\n\n", LANG_NAME, VERSION, VERSION_DATE);
    exit(0);
}

/**
 * Parses the passed arguments to the agent, and updates global variables in correspondence.
 *
 * @param argc the number of arguments sent to the program.
 * @param argv the arguments them self as sent to the program.
 */
void parseArguments(int argc, char* argv[]) {
    // Iterate over all sent arguments
    while (++argv, --argc) {
        // Commands begin with dash "-"
        if (**argv == '-') {
            // Print help menu
            if (strcmp(*argv, "-h") == 0 || strcmp(*argv, "--help") == 0) {
                printHelp();
            }
            // Print version
            else if (strcmp(*argv, "-v") == 0 || strcmp(*argv, "--version") == 0) {
                printVersion();
            }
            // Set output filename
            else if (strcmp(*argv, "-o") == 0 || strcmp(*argv, "--output") == 0) {
                if (--argc < 1) {
                    fprintf(stderr, "Error: Missing output filename!\n\n");
                    printHelp();
                }

                outputFilename = string(*(++argv));
            }
            // Invalid command
            else {
                fprintf(stderr, "Unknown argument '%s'\n", *argv);
            }
        }
        // Set input filename
        else if (inputFilename.empty()) {
            inputFilename = string(*argv);
        }
        // Other arguments
        else {
            fprintf(stderr, "Warning: too many arguments, '%s' ignored\n", *argv);
        }
    }

    // Check if input filename is specified
    if (inputFilename.empty()) {
        fprintf(stderr, "Error: Missing input filename argument!\n\n");
        printHelp();
    }
}
