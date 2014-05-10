all:
	cc -std=c99 -O0 -g -Wall src/*.c -ledit -o bin/clisp
