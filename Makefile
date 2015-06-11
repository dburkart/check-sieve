LEX = flex -I
YACC = bison -d

CC = clang -DYYDEBUG=1

check-sieve: sieve.tab.c lex.yy.c
	$(CC) -o check-sieve sieve.tab.c lex.yy.c -ly -ll -lm

sieve.tab.c: sieve.y
	$(YACC) sieve.y

lex.yy.c: sieve.l
	$(LEX) sieve.l

