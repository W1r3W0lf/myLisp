
LEX 	= flex
YACC	= bison
CC  	= gcc
CXX 	= g++

YFLAGS = -H -g -Wcounterexamples
CFLAGS = -Wall -g

LIBS = -lreadline -lfl

TEST_LIBS = -lcheck

objects = main.o scan.o parse.o symtable.o ast.o interp.o
test_objects = test.o ast.o check_ast.o symtable.o check_symtable.o check_interp.o interp.o check_parse.o parse.h parse.o scan.o


mylisp: $(objects)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^
main.o: parse.h
$(objects):
scan.c: scan.l parse.h
parse.c + parse.h: parse.y
	$(YACC) $(YFLAGS) parse.y -o parse.c
	dot -Tpng parse.gv -o parse.png


test: $(test_objects)
	$(CC) $(CFLAGS) $(LIBS) $(TEST_LIBS) -o $@ $^
	./$@


%.o: ./tests/%.c
	$(CC) $(CFLAGS) -c -o $@ $^


.PHONY: clean
clean:
	rm -r mylisp test $(objects) $(test_objects) parse.c parse.h scan.c parse.gv parse.png

