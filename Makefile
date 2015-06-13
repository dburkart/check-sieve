LEX = flex -I
YACC = bison -d

CC = clang++ -DYYDEBUG=1
CFLAGS = -Igen -Isrc

check-sieve: gen/sieve_parser.tab.cc gen/sieve_scanner.c src/sieve_driver.cc src/sieve.cc
	$(CC) $(CFLAGS) -c gen/sieve_scanner.c gen/sieve_parser.tab.cc src/sieve_driver.cc src/sieve.cc
	$(CC) $(CFLAGS) -o check-sieve sieve_scanner.o sieve_parser.tab.o sieve_driver.o sieve.o

gen/sieve_parser.tab.cc: src/sieve_parser.yy
	$(YACC) --file-prefix=gen/sieve_parser src/sieve_parser.yy

gen/sieve_scanner.c: src/sieve_scanner.l
	$(LEX) --header-file=gen/sieve_scanner.h --outfile gen/sieve_scanner.c src/sieve_scanner.l

clean:
	rm *.o gen/*