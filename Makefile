
LEX 	= flex
YACC	= bison
CC  	= gcc
CXX 	= g++

YFLAGS = -H
CFLAGS = -lreadline -Wall -g -lfl

objects = main.o scan.o parse.o deflist.o ast.o interp.o


mylisp: $(objects)
	$(CC) $(CFLAGS) -o $@ $^
main.o: parse.h
deflist.o:
ast.o:
interp.o:
scan.c: scan.l parse.h
parse.c + parse.h: parse.y
	$(YACC) $(YFLAGS) parse.y -o parse.c


.PHONY: clean
clean:
	rm -r mylisp $(objects) parse.c parse.h scan.c

