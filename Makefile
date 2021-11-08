
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
$(objects) :
scan.c: scan.l parse.h
parse.c + parse.h: parse.y
	$(YACC) $(YFLAGS) parse.y -o parse.c
	dot -Tpng parse.gv -o parse.png


test: ast_test
	./ast_test

ast_test: ast.o check_ast.o
	$(CC) -lcheck -g -Wall -o $@ $^

check_ast.o: ./tests/check_ast.c
	$(CC) -lcheck -g -Wall -c -o $@ $^



.PHONY: clean
clean:
	rm -r mylisp $(objects) parse.c parse.h scan.c parse.gv parse.png *.o ast_test

