BASE = .

include $(BASE)/Makefile.shared
include $(BASE)/gen/Makefile.env
include $(BASE)/src/Makefile.env

all: libchecksieve.a check-sieve

codegen:
	make -C $(BASE)/src codegen

check-sieve: $(BINARY_SRC)
	make -C $(BASE)/src binary
	$(CXX) $(CFLAGS) -o check-sieve $(BINARY_OBJ) libchecksieve.a

libchecksieve.a: $(GENERATED_SRC) $(LIBCHECKSIEVE_SRC)
	make -C $(BASE)/gen
	make -C $(BASE)/src lib
	ar rc libchecksieve.a $(GENERATED_OBJ) $(LIBCHECKSIEVE_OBJ)

clean:
	make -C $(BASE)/gen clean
	make -C $(BASE)/src clean
	rm -f libchecksieve.a check-sieve