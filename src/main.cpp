#include <iostream>
#include <string>
#include <string.h>

#include "context/scope_context.h"
#include "context/generation_context.h"
#include "parse_tree/parse_tree.h"
#include "utils/utils.h"
#include "utils/consts.h"

using namespace std;

//
// Compiler definitions
//
#define LANG_NAME           "M++"
#define VERSION             "1.0"
#define VERSION_DATE        "May 9, 2019"

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
string symbolTableFilename;
bool warn = false;

//
// Functions prototypes
//
void writeToFile(string data, string filename);
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

    // Construct context objects
    ScopeContext scopeContext(inputFilename, warn);
    GenerationContext genContext;

    // Open input file for Lex & Yacc
    yyin = fopen(inputFilename.c_str(), "r");

    if (yyin == NULL) {
        fprintf(stderr, "error: could not open the input file '%s'!\n", inputFilename.c_str());
        exit(0);
    }

    // Construct the parse tree
    yyparse();

    // Apply semantic check and quadruple generation
    if (programRoot != NULL && programRoot->analyze(&scopeContext)) {
        // cout << programRoot->toString() << endl;
        writeToFile(programRoot->generateQuad(&genContext), outputFilename);
        writeToFile(scopeContext.getSymbolTableStr(), symbolTableFilename);
    } else {
        writeToFile("", outputFilename);
    }

    // Finalize and release allocated memory
    fclose(yyin);

    if (programRoot != NULL) {
        delete programRoot;
    }

    return 0;
}

/**
 * Creates a new file and writes the given data into it.
 *
 * @param data     the data to write.
 * @param filename the filename of the file to write into.
 */
void writeToFile(string data, string filename) {
    if (filename.empty()) {
        return;
    }

    ofstream fout(filename);

    if (!fout.is_open()) {
        fprintf(stderr, "error: could not write in file '%s'!\n", filename.c_str());
        return;
    }

    fout << data << endl;

    fout.close();
}

/**
 * Prints the help menu of the compiler into the
 * standard output stream, then terminates the program.
 */
void printHelp() {
    printf("%s version %s, %s\n\n", LANG_NAME, VERSION, VERSION_DATE);
    printf("Usage: %s [switches] <input_file>\n", LANG_NAME);
    printf("    -h, --help                   Print the help menu and exit.\n");
    printf("    -o, --output <filename>      Specify the output filename.\n");
    printf("    -s, --sym_table <filename>   Output the symbol table to the given file\n");
    printf("    -v, --version                Print the installed version number and exit.\n");
    printf("    -w, --warn                   Show warning messages.\n");
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
            // Show warnings
            else if (strcmp(*argv, "-w") == 0 || strcmp(*argv, "--warn") == 0) {
                warn = true;
            }
            // Set output filename
            else if (strcmp(*argv, "-o") == 0 || strcmp(*argv, "--output") == 0) {
                if (--argc < 1) {
                    fprintf(stderr, "error: missing output filename!\n\n");
                    printHelp();
                }

                outputFilename = string(*(++argv));
            }
            // Set symbol table output filename
            else if (strcmp(*argv, "-s") == 0 || strcmp(*argv, "--sym_table") == 0) {
                if (--argc < 1) {
                    fprintf(stderr, "error: missing output filename!\n\n");
                    printHelp();
                }

                symbolTableFilename = string(*(++argv));
            }
            // Invalid command
            else {
                fprintf(stderr, "unknown argument '%s'\n", *argv);
            }
        }
        // Set input filename
        else if (inputFilename.empty()) {
            inputFilename = string(*argv);
        }
        // Other arguments
        else {
            fprintf(stderr, "warning: too many arguments, '%s' ignored\n", *argv);
        }
    }

    // Check if input filename is specified
    if (inputFilename.empty()) {
        fprintf(stderr, "error: missing input filename argument!\n\n");
        printHelp();
    }
}
