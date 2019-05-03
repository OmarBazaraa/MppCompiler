clear:
	cls

copy:
	xcopy src out /E /y /q

gen:
	flex -o out/rules/lexer.cpp out/rules/lexer_specs.l
	bison -d -o out/rules/parser.cpp out/rules/parser_grammar.y

comp:
	g++ -o out/M++.exe out/main.cpp out/rules/lexer.cpp out/rules/parser.cpp

build:
	@make -s clear
	@make -s copy
	@make -s gen
	@make -s comp

run:
	@make -s clear
	out\\M++.exe data/input.mpp

all:
	@make -s build
	@make -s run