
LEX 	= flex
YACC	= bison
CC		= gcc
CXX		= g++

CFLAGS =

objects = main.o

add: lex.yy.c add.tab.c

	$(CC) add.tab.c lex.yy.c   -o add -lfl

lex.yy.c:
	$(LEX) add.l

add.tab.c:
	$(YACC) -d add.y


.PHONY: clean
clean:
	rm -r add add.tab.* lex.yy.c

