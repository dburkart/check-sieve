BASE = ..

include $(BASE)/Makefile.shared
include $(BASE)/src/Makefile.env

all: $(BINARY_OBJ) $(LIBCHECKSIEVE_OBJ)
	
binary: $(BINARY_OBJ)

lib: $(LIBCHECKSIEVE_OBJ)

codegen: $(BASE)/src/sieve_parser.yy $(BASE)/src/sieve_scanner.l
	$(YACC) --file-prefix=$(BASE)/gen/sieve_parser $(BASE)/src/sieve_parser.yy
	$(LEX) --header-file=$(BASE)/gen/sieve_scanner.hh --outfile $(BASE)/gen/sieve_scanner.cc $(BASE)/src/sieve_scanner.l

clean:
	rm -f $(BINARY_OBJ) $(LIBCHECKSIEVE_OBJ)

$(BINARY_OBJ): %.o: %.cc
	$(CXX) -c $(CFLAGS) $< -o $@

$(LIBCHECKSIEVE_OBJ): %.o: %.cc
	$(CXX) -c $(CFLAGS) $< -o $@