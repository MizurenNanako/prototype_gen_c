CC = gcc

debug : CC = gcc -g
debug : main

release : main

main : main.o help.o options.o stack.o parse.o

clean : 
	rm *.o

test.h : stdafx.h options.c stack.c parse.c err.h
main.c : stdafx.h test.h

help.h : stdafx.h
help.c : help.h 

options.h : stdafx.h
options.c : options.h err.h help.c

stack.h : stdafx.h 
stack.c : stack.h err.h

parse.h : stdafx.h
parse.c : parse.h err.h