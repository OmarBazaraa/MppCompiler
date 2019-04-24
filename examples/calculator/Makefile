clear:
	cls

copy:
	copy src\\symbol_table.h out\\symbol_table.h

lex_gen:
	flex -o out/lexer.c src/lexer_specs.l

lex_build:
	gcc out/lexer.c -o out/lexer.exe

lex_run:
	out\\lexer.exe < data/input.txt

lex_all:
	@make -s clear
	@make -s lex_gen
	@make -s lex_build
	@make -s lex_run

yacc_gen:
	@make -s lex_gen
	bison -d -o out/parser.c src/parser_grammer.y

yacc_build:
	gcc -c out/lexer.c -o out/lexer.o
	gcc -c out/parser.c -o out/parser.o
	gcc -o out/parser.exe out/lexer.o out/parser.o

yacc_run:
	out\\parser.exe

yacc_all:
	@make -s clear
	@make -s copy
	@make -s yacc_gen
	@make -s yacc_build
	@make -s yacc_run