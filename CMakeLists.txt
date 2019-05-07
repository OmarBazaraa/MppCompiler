cmake_minimum_required(VERSION 3.13)
project(MppCompiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(MppCompiler
        src/main.cpp

        src/parse_tree/statements/statement_analyzer.cpp
        src/parse_tree/statements/statement_generator.cpp

        src/parse_tree/expressions/expression_analyzer.cpp
        src/parse_tree/expressions/expression_generator.cpp
        src/parse_tree/expressions/expression_evaluator.cpp

        src/parse_tree/branches/branch_analyzer.cpp
        src/parse_tree/branches/branch_generator.cpp

        src/parse_tree/functions/function_analyzer.cpp
        src/parse_tree/functions/function_generator.cpp

        src/parser/lexer.cpp
        src/parser/parser.cpp
)


add_custom_target(
        gen_lexer ALL
        COMMAND flex -o ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/rules/lexer_specs.l
        BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/lexer.cpp
        COMMENT "Generating lexer"
)

add_custom_target(
        gen_parser ALL
        COMMAND bison -d -o ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/rules/parser_grammar.y
        BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.cpp
        COMMENT "Generating parser"
)

add_dependencies(MppCompiler gen_lexer gen_parser)