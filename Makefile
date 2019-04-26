clear:
	cls

copy:
	xcopy src out /E /y /q

gen:
	flex -o out/rules/lexer.cpp out/rules/lexer_specs.l
	bison -d -o out/rules/parser.cpp out/rules/parser_grammar.y

build:
	g++ -std=c++14 -c out/rules/lexer.cpp -o out/lexer.o
	g++ -std=c++14 -c out/rules/parser.cpp -o out/parser.o
	g++ -std=c++14 -o out/parser.exe out/lexer.o out/parser.o

run:
	out\\parser.exe < data/input.txt

all:
	@make -s clear
	@make -s copy
	@make -s gen
	@make -s build
	@make -s run