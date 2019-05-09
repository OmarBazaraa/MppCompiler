clear:
	cls

copy:
	xcopy src out /E /y /q

gen:
	flex -o out/rules/lexer.cpp out/rules/lexer_specs.l
	bison -d -o out/rules/parser.cpp out/rules/parser_grammar.y

comp:
	g++ -o out/M++.exe \
		out/main.cpp \
		\
		out/parse_tree/statements/statement_analyzer.cpp \
		out/parse_tree/statements/statement_generator.cpp \
		\
		out/parse_tree/expressions/expression_analyzer.cpp \
		out/parse_tree/expressions/expression_generator.cpp \
		out/parse_tree/expressions/expression_evaluator.cpp \
		\
		out/parse_tree/branches/branch_analyzer.cpp \
		out/parse_tree/branches/branch_generator.cpp \
		\
		out/parse_tree/functions/function_generator.cpp \
		out/parse_tree/functions/function_analyzer.cpp \
		\
		out/rules/lexer.cpp \
		out/rules/parser.cpp

build:
	@make -s clear
	@make -s copy
	@make -s gen
	@make -s comp

run:
	@make -s clear
	out\\M++.exe data/input.mpp -o data/out.quad -s data/symbol_table.txt

all:
	@make -s build
	@make -s run