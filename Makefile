clear:
	cls

lex_gen:
	flex -o out/lex.c src/lex_specs.l

lex_build:
	gcc out/lex.c -o out/lex.exe

lex_run:
	out\\lex.exe < data/input.txt

lex_all:
	@make -s clear
	@make -s lex_gen
	@make -s lex_build
	@make -s lex_run