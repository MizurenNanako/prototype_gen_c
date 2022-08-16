CC = gcc

debug : CC = gcc -g
debug : main

release : main

main : main.o help.o options.o

clean : 
	rm *.o
