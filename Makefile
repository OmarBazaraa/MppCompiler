clear:
	cls

copy:
	copy src\\parse_tree.h out\\parse_tree.h

gen:
	flex -o out/lexer.cpp src/lexer_specs.l
	bison -d -o out/parser.cpp src/parser_grammer.y

build:
	g++ -std=c++14 -c out/lexer.cpp -o out/lexer.o
	g++ -std=c++14 -c out/parser.cpp -o out/parser.o
	g++ -std=c++14 -o out/parser.exe out/lexer.o out/parser.o

run:
	out\\parser.exe < data/input.txt

all:
	@make -s clear
	@make -s copy
	@make -s gen
	@make -s build
	@make -s run