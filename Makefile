clear:
	cls

copy:
	xcopy src out /E /y /q

gen:
	flex -o out/rules/lexer.cpp out/rules/lexer_specs.l
	bison -d -o out/rules/parser.cpp out/rules/parser_grammar.y

comp:
	g++ -std=c++14 -c out/rules/lexer.cpp -o out/lexer.o
	g++ -std=c++14 -c out/rules/parser.cpp -o out/parser.o
	g++ -std=c++14 -c out/main.cpp -o out/main.o
	g++ -std=c++14 -o out/M++.exe out/main.o out/lexer.o out/parser.o

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