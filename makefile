CC = gcc

debug : CC = gcc -g
debug : main

release : main

main : main.o help.o options.o stack.o parse.o list.o hash.o dictionary.o

clean : 
	rm *.o

err.h : colordef.h

test.h : stdafx.h options.c stack.c parse.c list.c err.h 
main.c : stdafx.h test.h

help.h : stdafx.h
help.c : help.h colordef.h

options.h : stdafx.h
options.c : options.h err.h help.c

stack.h : stdafx.h 
stack.c : stack.h err.h

parse.h : stdafx.h
parse.c : parse.h err.h

list.h : stdafx.h
list.c : list.h err.h

hash.h : stdafx.h
hash.c : hash.h err.h

dictionary.h : stdafx.h
dictionary.c : dictionary.h list.h err.h

_line_metadata.h : stdafx.h
_line_metadata.c : _line_metadata.h list.h err.h
