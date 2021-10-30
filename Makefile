
LEX 	= flex
YACC	= bison
CC		= gcc
CXX		= g++

CFLAGS = -lreadline -Wall -g -lfl -lbison

objects = main.o

mylisp: $(objects)
	$(CC) $(CFLAGS) -o mylisp $(objects)

main.o:


.PHONY: clean
clean:
	rm -r mylisp $(objects)

