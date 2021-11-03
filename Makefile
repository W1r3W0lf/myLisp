
LEX 	= flex
YACC	= bison
CC  	= gcc
CXX 	= g++

YFLAGS = -H -g
CFLAGS = -lreadline -Wall -g -lfl

objects = main.o scan.o parse.o symtable.o ast.o interp.o


mylisp: $(objects)
	$(CC) $(CFLAGS) -o $@ $^
main.o: parse.h
symtable.o:
ast.o:
interp.o:
scan.c: scan.l parse.h
parse.c + parse.h: parse.y
	$(YACC) $(YFLAGS) parse.y -o parse.c
	dot -Tpng parse.gv -o parse.png


# I'm thinking of writting some unit tests


.PHONY: clean
clean:
	rm -r mylisp $(objects) parse.c parse.h scan.c

