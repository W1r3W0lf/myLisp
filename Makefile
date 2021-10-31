
LEX 	= flex
YACC	= bison
CC		= gcc
CXX		= g++

CFLAGS = -lreadline -Wall -g -lfl -lbison

objects = main.o scan.o parse.o

mylisp: $(objects)
main.o:
scan.o: scan.l parse.c
parse.o: parse.y

.PHONY: clean
clean:
	rm -r mylisp $(objects)

