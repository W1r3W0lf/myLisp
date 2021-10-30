
LEX 	= flex
YACC	= bison
CC		= gcc
CXX		= g++

objects = main.o

mylisp: $(objects)
	$(CC) -o mylisp $(objects)

main.o:


.PHONY: clean
clean:
	rm -r mylisp $(objects)

