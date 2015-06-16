LEX = flex -I
YACC = bison -d

CC = clang++ -DYYDEBUG=1
CFLAGS = -Igen/ -Isrc/

check-sieve: libchecksieve.a src/sieve.cc
	$(CC) $(CFLAGS) -c src/sieve.cc
	$(CC) $(CFLAGS) libchecksieve.a sieve.o -o check-sieve

all: codegen check-sieve test

libchecksieve.a: gen/sieve_parser.tab.cc gen/sieve_scanner.c src/sieve_driver.cc
	$(CC) $(CFLAGS) -c gen/sieve_parser.tab.cc gen/sieve_scanner.c src/sieve_driver.cc
	ar rc libchecksieve.a sieve_scanner.o sieve_parser.tab.o sieve_driver.o

codegen: src/sieve_parser.yy src/sieve_scanner.l
	$(YACC) --file-prefix=gen/sieve_parser src/sieve_parser.yy
	$(LEX) --header-file=gen/sieve_scanner.h --outfile gen/sieve_scanner.c src/sieve_scanner.l

checksieve.so: src/python.cc libchecksieve.a
	python test/setup.py build_ext -i

test: FORCE libchecksieve.a checksieve.so
	python -m unittest discover -s test -p '*_test.py'

FORCE:

clean:
	rm *.o checksieve.so libchecksieve.a check-sieve